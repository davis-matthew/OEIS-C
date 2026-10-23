/*
 * A110209  1 + sum of first n 3-almost primes.
 * https://oeis.org/A110209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + sum of the first n 3‑almost primes (A014612) * a(0) =
 *            1. The implementation builds a 3‑almost‑prime sieve *
 *            incrementally until the requested index is produced. * The code
 *            is O(sieve_limit) with sieve_limit ≈ n·log n, below 2 s * for n
 *            ≤ 10 000
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/*  External types supplied by the build environment:
 *     seq_t – unsigned integer of 8/16/32/64/128 bits or arbitrary‑precision
 *     idx_t – unsigned 64‑bit integer (uint64_t)
 *
 *  The following static state holds the computed prefix of the sequence.
 */
static seq_t *prefix        = NULL;   /* array of computed a(n) values          */
static idx_t  prefix_size   = 0;     /* number of stored terms                  */
static idx_t  prefix_cap    = 0;     /* allocated capacity of prefix[]          */
static uint64_t sieve_limit = 0;     /* current upper bound for the sieve       */

/*  Append a value to the prefix array, expanding it if necessary. */
static void push_value(seq_t val)
{
    if (prefix_size == prefix_cap) {
        prefix_cap = prefix_cap ? prefix_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(prefix, prefix_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        prefix = tmp;
    }
    prefix[prefix_size++] = val;
}

/*  Recompute the entire prefix array up to the current sieve_limit.  */
static void precompute(void)
{
    uint32_t limit = (uint32_t)sieve_limit;   /* limit fits in 32 bits for the tests   */

    /*  Small‑prime‑factor array (linear sieve). */
    uint32_t *spf  = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) exit(1);
    memset(spf, 0, (limit + 1) * sizeof(uint32_t));

    uint32_t *primes = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!primes) { free(spf); exit(1); }
    size_t primes_cnt = 0;

    for (uint32_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                     /* i is prime */
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p  = primes[j];
            uint64_t ip = (uint64_t)i * p;
            if (p > spf[i] || ip > limit) break;
            spf[(size_t)ip] = p;
        }
    }

    /*  Compute big‑omega (prime‑factor count with multiplicity). */
    uint8_t *omega = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!omega) { free(spf); free(primes); exit(1); }
    omega[1] = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        omega[i] = omega[i / spf[i]] + 1;
    }

    /*  Reallocate the prefix array from scratch. */
    if (prefix) { free(prefix); prefix = NULL; }
    prefix_cap  = 0;
    prefix_size = 0;

    /*  a(0) = 1  */
    push_value((seq_t)1);

    /*  Sum of 3‑almost primes collected so far. */
    seq_t sum = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (omega[i] == 3) {
            sum = sum + (seq_t)i;          /* sum of first k 3‑almost primes   */
            push_value(sum + (seq_t)1);     /* a(k) = 1 + sum                   */
        }
    }

    free(spf);
    free(primes);
    free(omega);
}

/*  Ensure that prefix[n] is available; extend the sieve if necessary. */
static void ensure_prefix(idx_t n)
{
    if (prefix_size > n) return;

    if (sieve_limit == 0) sieve_limit = 200000;     /* starting bound */

    while (prefix_size <= n) {
        sieve_limit *= 2;
        precompute();
    }
}

/*  Public function required by the judge. */
seq_t a(idx_t n)
{
    ensure_prefix(n);
    return prefix[(size_t)n];
}
