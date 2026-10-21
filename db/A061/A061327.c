/*
 * A061327  Primes with 14 as smallest positive primitive root.
 * https://oeis.org/A061327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_primitive_root, mod_pow_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061327: primes with 14 as smallest positive primitive root.
   Computed by iterating primes and testing primitive‑root conditions. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_primitive_root.h"
#include "../utils/mod_pow_uint32.h"

/*---  small prime sieve up to 6000 -----------------------------------------*/
static int small_primes[900];
static int small_cnt = 0;

static void init_small_primes(void)
{
    const int N = 6000;
    char *comp = (char *)calloc(N + 1, 1);
    if (!comp) exit(1);
    for (int i = 2; i <= N; ++i) {
        if (!comp[i]) {
            if (small_cnt < 900) small_primes[small_cnt++] = i;
            for (int j = i * i; j <= N; j += i) comp[j] = 1;
        }
    }
    free(comp);
}

/*---  linear sieve: primes up to LIMIT -------------------------------------*/
static int *primes = NULL;
static size_t primes_size = 0, primes_cap = 0;
static const int LIMIT = 30000000;     /* enough for 10 000 terms */

static void init_primes(void)
{
    char *comp = (char *)calloc(LIMIT + 1, 1);
    if (!comp) exit(1);

    primes_cap = 2000000;               /* initial capacity */
    primes = (int *)malloc(primes_cap * sizeof(int));
    if (!primes) exit(1);

    for (int i = 2; i <= LIMIT; ++i) {
        if (!comp[i]) {
            if (primes_size == primes_cap) {
                primes_cap *= 2;
                primes = (int *)realloc(primes, primes_cap * sizeof(int));
                if (!primes) exit(1);
            }
            primes[primes_size++] = i;
        }
        for (size_t j = 0; j < primes_size; ++j) {
            long long prod = 1LL * i * primes[j];
            if (prod > LIMIT) break;
            comp[(int)prod] = 1;
            if (primes[j] == i) break;
        }
    }
    free(comp);
}

/*---  factorised n-1 (unique prime factors) ------------------------------*/
static void get_factors(uint32_t n, int *factors, int *cnt)
{
    *cnt = 0;
    uint32_t temp = n;
    for (int i = 0; i < small_cnt && (uint32_t)small_primes[i] * (uint32_t)small_primes[i] <= temp; ++i) {
        int p = small_primes[i];
        if (temp % p == 0) {
            factors[(*cnt)++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) factors[(*cnt)++] = (int)temp;   /* remaining prime factor > sqrt */
}

/*---  sequence storage -----------------------------------------------------*/
static int *seq_arr = NULL;
static size_t seq_size = 0, seq_cap = 0;
static size_t prime_idx = 0;

/*---  pre‑initialisation ---------------------------------------------------*/
static int initialized = 0;
static void ensure_preinit(void)
{
    if (initialized) return;
    init_small_primes();
    init_primes();
    seq_cap = 128;
    seq_arr = (int *)malloc(seq_cap * sizeof(int));
    if (!seq_arr) exit(1);
    initialized = 1;
}

/*---  extend the sequence to at least n terms ------------------------------*/
static void extend_sequence(size_t n)
{
    int factors[32];
    int fcnt;

    while (seq_size < n) {
        if (prime_idx >= primes_size) {
            /* Should not happen for the required limits. */
            break;
        }
        int p = primes[prime_idx++];
        if (p <= 13) continue;          /* 14 must be coprime to p */

        get_factors((uint32_t)p - 1, factors, &fcnt);

        if (!is_primitive_root((uint32_t)p, 14, factors, fcnt)) continue;

        int ok = 1;
        for (int g = 2; g < 14; ++g) {
            if (is_primitive_root((uint32_t)p, g, factors, fcnt)) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            if (seq_size == seq_cap) {
                seq_cap *= 2;
                seq_arr = (int *)realloc(seq_arr, seq_cap * sizeof(int));
                if (!seq_arr) exit(1);
            }
            seq_arr[seq_size++] = p;
        }
    }
}

/*---  public function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n=0, but safe guard */
    ensure_preinit();
    extend_sequence((size_t)n);
    return (seq_t)seq_arr[(size_t)n - 1];
}
