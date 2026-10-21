/*
 * A085760  Prime powers of the form 4n+3.
 * https://oeis.org/A085760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085760: prime powers of the form 4n+3.  
   a(n) is the n‑th smallest integer of the form p^(2k+1), where p is a
   prime with p ≡ 3 (mod 4) and k ≥ 0. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* comparison function for qsort on seq_t values */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

/* static state: generated powers, count, and the bound used last */
static seq_t *powers = NULL;
static size_t powers_cnt = 0;
static seq_t max_value = 0;     /* upper bound used for generation */

/* perform a sieve up to 'limit', returning primes in 'primes' array.
   The caller must free 'primes' and the temporary array. */
static void sieve_primes(idx_t limit, idx_t **primes, size_t *cnt)
{
    /* simple boolean sieve */
    bool *comp = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!comp) {
        *primes = NULL;
        *cnt = 0;
        return;
    }
    memset(comp, 0, (limit + 1) * sizeof(bool));
    comp[0] = comp[1] = true;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (!comp[i])
            for (idx_t j = i * i; j <= limit; j += i)
                comp[j] = true;

    /* first count primes ≡ 3 (mod 4) */
    size_t p_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[i] && (i % 4 == 3))
            ++p_cnt;

    idx_t *list = (idx_t *)malloc(p_cnt * sizeof(idx_t));
    if (!list) {
        free(comp);
        *primes = NULL;
        *cnt = 0;
        return;
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[i] && (i % 4 == 3))
            list[idx++] = i;

    free(comp);
    *primes = list;
    *cnt = p_cnt;
}

/* Ensure that at least 'n' terms have been generated.  This function
   repeatedly enlarges the generation bound until enough terms are
   available.  The generation is performed from scratch each time to
   keep the implementation simple. */
static void ensure_seq(idx_t n)
{
    if (powers_cnt >= (size_t)n)
        return;                /* already enough */

    /* initial generation bound */
    if (max_value == 0)
        max_value = 1000;

    while (powers_cnt < (size_t)n) {
        /* increase bound (doubling strategy) */
        if (max_value > (seq_t)UINT64_MAX / 2)
            break; /* cannot increase further; stop */
        max_value *= 2;
        idx_t limit = (idx_t)max_value;

        /* compute primes ≡ 3 (mod 4) up to 'limit' */
        idx_t *primes = NULL;
        size_t primes_cnt = 0;
        sieve_primes(limit, &primes, &primes_cnt);
        if (!primes) {
            /* out of memory; abort further generation */
            break;
        }

        /* first pass: count how many prime powers ≤ limit */
        size_t count = 0;
        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            seq_t p  = (seq_t)primes[pi];
            seq_t val = p;          /* exponent 1 (odd) */
            seq_t pp2 = p * p;      /* p squared, used to step exponents by 2 */
            while (true) {
                ++count;
                if (val > limit / pp2)
                    break;
                val *= pp2;
            }
        }

        /* if count still < n, continue with larger bound */
        if (count < (size_t)n) {
            free(primes);
            continue;
        }

        /* second pass: generate the values */
        seq_t *arr = (seq_t *)malloc(count * sizeof(seq_t));
        if (!arr) {
            free(primes);
            break; /* memory failure */
        }
        size_t idx = 0;
        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            seq_t p  = (seq_t)primes[pi];
            seq_t val = p;          /* odd exponent 1 */
            seq_t pp2 = p * p;
            while (true) {
                arr[idx++] = val;
                if (val > limit / pp2)
                    break;
                val *= pp2;
            }
        }

        /* sort the array */
        qsort(arr, count, sizeof(seq_t), cmp_seq_t);

        /* free old data and replace with new */
        free(powers);
        free(primes);
        powers = arr;
        powers_cnt = count;
        /* generation successful; exit */
        break;
    }
}

/* Return the nth element of the sequence (1‑based).  If n == 0, 0 is
   returned as a placeholder, but the sequence is defined only for n≥1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    ensure_seq(n);
    return powers[n - 1];
}
