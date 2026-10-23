/*
 * A124268  Primes indexed by 3-almost primes.
 * https://oeis.org/A124268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(3almostprime(n))
 * Range:     u8 n <= 12, u16 n <= 1676, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A124268:  primes indexed by 3‑almost primes. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_spf.h"   /* build_spf(limit, &spf) */
#include "../utils/get_prime.h"   /* get_prime(k) */

static idx_t *spf = NULL;            /* smallest prime factor array     */
static const idx_t spf_limit = 2000000;  /* sufficient for n up to ~10⁴    */

static idx_t *three_idx = NULL;      /* storage for discovered 3‑almost primes */
static size_t three_cap = 0;         /* capacity of three_idx               */
static size_t three_count = 0;       /* number of 3‑almost primes found    */
static idx_t last_checked = 1;       /* last integer examined for ω(x)=3   */

/* Ensure the SPF table is initialized once. */
static void init_spf(void)
{
    if (spf != NULL)
        return;
    build_spf(spf_limit, &spf);
}

/* Ensure that three_idx has room for at least `needed` elements. */
static void ensure_three_capacity(size_t needed)
{
    if (three_cap >= needed)
        return;
    size_t new_cap = three_cap ? (three_cap << 1) : 128;
    while (new_cap < needed)
        new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(three_idx, new_cap * sizeof(idx_t));
    if (!tmp)
        abort();                     /* out of memory */
    three_idx = tmp;
    three_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* undefined for offset 0, harmlessly return 0 */

    init_spf();     /* build SPF table once */

    /* Generate 3‑almost primes until we have at least n of them. */
    while (three_count < n)
    {
        ++last_checked;

        /* Count prime factors with multiplicity using the SPF array. */
        idx_t x = last_checked;
        unsigned cnt = 0;
        while (x > 1)
        {
            x /= spf[x];
            ++cnt;
            if (cnt > 3)
                break;
        }
        if (cnt == 3)
        {
            ensure_three_capacity(three_count + 1);
            three_idx[three_count++] = last_checked;
        }
    }

    /* Retrieve the n‑th 3‑almost prime (1‑based). */
    idx_t m = three_idx[n - 1];

    /* The answer is the m‑th prime. */
    return get_prime(m);
}
