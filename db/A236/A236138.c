/*
 * A236138  a(n) = |{0 < k < n: p = phi(k) + phi(n-k)/3 - 1, prime(p-1) -
 *          (p-1) and prime(p-1) - 2*prime((p-1)/2) are all prime}|, where
 *          phi(.) is Euler's totient function.
 * https://oeis.org/A236138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k < n : p = φ(k) + φ(n-k)/3 - 1, with φ the Euler
 *            totient; prime(p-1) - (p-1) and prime(p-1) - 2·prime((p-1)/2)
 *            both prime}| The algorithm fills a φ array up to the requested n
 *            (sieve of Euler) and then scans all k. It rejects any k for
 *            which φ(n-k) is not divisible by 3, because in...
 * Range:     u8 n <= 95, u16 n <= 8204, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9206 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <stddef.h>
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

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* storage for Euler's totient values */
static seq_t *phi_arr = NULL;
static idx_t  phi_max = 0;

/* build φ[0 … n] */
static void build_phi(idx_t n)
{
    if (phi_arr == NULL) {
        phi_arr = mem_new(seq_t, (n + 1));
    } else if (n > phi_max) {
        phi_arr = mem_resize(seq_t, phi_arr, (n + 1));
    } else {
        return;
    }

    for (idx_t i = 0; i <= n; ++i) phi_arr[i] = (seq_t)i;

    for (idx_t p = 2; p <= n; ++p) {
        if (phi_arr[p] == (seq_t)p) {          /* p is prime */
            for (idx_t j = p; j <= n; j += p)
                phi_arr[j] -= phi_arr[j] / (seq_t)p;
        }
    }
    phi_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* out of range safety */

    if (n > phi_max) build_phi(n);          /* ensure φ up to n */

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        seq_t pk   = phi_arr[k];
        seq_t pnk  = phi_arr[n - k];
        /* φ(n-k) must be divisible by 3 */
        if (pnk % (seq_t)3 != 0) continue;
        seq_t p = pk + pnk / (seq_t)3 - (seq_t)1;
        if (p < (seq_t)2) continue;         /* p must be prime */

        /* p itself must be prime */
        if (!is_prime_big(p)) continue;

        /* p-1 must be a valid prime index */
        if (p <= (seq_t)1) continue;
        idx_t p_minus1 = (idx_t)(p - (seq_t)1);

        seq_t prime_p_minus1 = get_prime(p_minus1);
        seq_t val1 = prime_p_minus1 - (seq_t)p_minus1;
        if (val1 < (seq_t)2 || !is_prime_big(val1)) continue;

        /* (p-1)/2 must be integer and >= 1 */
        if ((p_minus1 & (seq_t)1) != 0) continue;  /* odd */
        idx_t half = (idx_t)(p_minus1 / (seq_t)2);
        if (half < (idx_t)1) continue;

        seq_t prime_half = get_prime(half);
        seq_t val2 = prime_p_minus1 - (seq_t)2 * prime_half;
        if (val2 < (seq_t)2 || !is_prime_big(val2)) continue;

        ++count;
    }
    return count;
}
