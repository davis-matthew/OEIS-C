/*
 * A046062  Primes of the form n*phi(n)+1 where phi(n) is the Euler function.
 * https://oeis.org/A046062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 94, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 975 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046062: primes of the form n*phi(n)+1
   a(n) = the nth prime p such that p = n*phi(n)+1 for some n ≥ 1.
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/*-------------------------------------------------------------
 *  Sieve for Euler's totient up to a fixed limit
 *-------------------------------------------------------------*/
#define MAX_LIMIT 2000000UL  /* sufficient for n≤10000 */

static idx_t *phi_arr = NULL;   /* phi[m] for 0..MAX_LIMIT */
static bool phi_ready = false;

/* initialise phi_arr with a linear sieve */
static void init_phi(void)
{
    if (phi_ready)
        return;
    phi_arr = (idx_t*)malloc((MAX_LIMIT + 1) * sizeof(idx_t));
    if (!phi_arr) abort();

    /* allocate room for the prime list */
    idx_t *primes = (idx_t*)malloc((MAX_LIMIT / 10 + 10) * sizeof(idx_t));
    if (!primes) abort();
    size_t pc = 0;

    phi_arr[0] = 0;
    phi_arr[1] = 1;
    for (idx_t i = 2; i <= MAX_LIMIT; ++i) {
        if (phi_arr[i] == 0) {            /* i is prime */
            phi_arr[i] = i - 1;
            primes[pc++] = i;
        }
        for (size_t j = 0; j < pc; ++j) {
            idx_t p = primes[j];
            idx_t v = i * p;
            if (v > MAX_LIMIT)
                break;
            if (i % p == 0) {
                phi_arr[v] = phi_arr[i] * p;
                break;
            } else {
                phi_arr[v] = phi_arr[i] * (p - 1);
            }
        }
    }
    free(primes);
    phi_ready = true;
}

/*-------------------------------------------------------------
 *  Cache of the sequence values
 *-------------------------------------------------------------*/
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ensure cache has at least 'need' slots */
static void grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t ncap = (cache_cap == 0) ? 4 : cache_cap;
    while (ncap < need)
        ncap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, ncap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = ncap;
}

/*-------------------------------------------------------------
 *  Main sequence function
 *-------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for the OEIS offset */
        return 0;

    init_phi();

    static idx_t next_m = 1;   /* the next m to test */

    while (cache_len < (size_t)n) {
        if (next_m > MAX_LIMIT) abort();  /* safety; should never happen */

        idx_t phi_m = phi_arr[next_m];
        seq_t candidate = (seq_t)next_m * (seq_t)phi_m + 1;

        if (is_prime_big(candidate)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = candidate;
        }
        ++next_m;
    }
    return cache[n - 1];   /* a(1) is the first stored value */
}
