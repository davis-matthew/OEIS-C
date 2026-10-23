/*
 * A144764  Partial totient function phi(c, n) for c = 3: number of
 *          3-semiprimes less than and coprime to n.
 * https://oeis.org/A144764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of 3‑semiprimes < n that are coprime to n.
 *            3‑semiprimes are numbers whose total number of prime factors
 *            (with multiplicity) equals 3
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */
/* The following utilities are provided by the build system:
 *
 * seq_t          – integer type (uint8_t … unsigned __int128 or an
 *                   arbitrary‑precision class)
 * idx_t          – index type, defined as uint64_t
 * seq_t gcd(seq_t a, seq_t b) – greatest common divisor
 *
 * Only these names are used from the utils.                        */
/* ------------------------------------------------------------------ */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* ------------------------------------------------------------------
 * Smallest prime factor (spf) sieve.
 * spf[i] = smallest prime dividing i (i >= 2). 0 for i < 2.
 * ------------------------------------------------------------------ */
static idx_t *spf = NULL;          /* array of spf values */
static idx_t  spf_lim = 0;         /* current size of spf (exclusive) */

static void ensure_spf(idx_t limit)
{
    if (limit < 2 || spf_lim >= limit + 1)
        return;

    /* free previous array if any */
    free(spf);

    /* allocate new array */
    spf = (idx_t*)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!spf) exit(1);

    /* initialise */
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;

    /* sieve */
    for (idx_t i = 2; i <= limit / i; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            for (idx_t j = i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }

    /* remaining primes > sqrt(limit) */
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == 0) spf[i] = i;

    spf_lim = limit + 1;
}

/* ------------------------------------------------------------------
 * List of 3‑semiprimes up to a given limit.
 * ------------------------------------------------------------------ */
static seq_t *semis = NULL;            /* array of 3‑semiprimes */
static size_t semis_cnt = 0;           /* number stored */
static idx_t  semis_lim = 0;           /* current limit (exclusive) */

static void recompute_semis(idx_t limit)
{
    free(semis);
    semis = (seq_t*)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!semis) exit(1);

    semis_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        idx_t x = i;
        int   omega = 0;
        while (x > 1 && omega <= 3) {
            ++omega;
            x /= spf[x];
        }
        if (omega == 3)
            semis[semis_cnt++] = (seq_t)i;
    }
    semis_lim = limit + 1;
}

/* ------------------------------------------------------------------
 * Main sequence function.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* offset 1 – a(1) = 0 */

    idx_t limit = n - 1;           /* we need 3‑semiprimes < n */

    /* Ensure spf and semis cover the needed range */
    ensure_spf(limit);
    if (limit >= semis_lim)
        recompute_semis(limit);

    seq_t count = 0;

    /* Count coprime semiprimes */
    for (size_t i = 0; i < semis_cnt; ++i) {
        seq_t m = semis[i];
        if (m >= (seq_t)n)    /* semis is sorted */
            break;
        if (gcd(m, (seq_t)n) == 1)
            ++count;
    }
    return count;
}
