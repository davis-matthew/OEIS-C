/*
 * A120438  Average of twin-prime pairs modulo 10 (least absolute residue).
 * https://oeis.org/A120438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least absolute residue of the average of the n‑th
 *            twin‑prime pair, where the average is (p + (p+2))/2 = p+1 and
 *            the residue is taken modulo 10 in the range –5…5
 * Range:     i8 n <= 111, i16 n <= 111, i32 n <= 111, i64 n <= 111,
 *            i128 n <= 111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a cache of all a(1)…a(k) that have been computed so far.
   It builds a simple sieve of Eratosthenes up to a limit large enough to contain the
   requested n‑th twin prime.  The limit is doubled until sufficient twin primes are
   found.  Each new limit rebuilds the sieve and the residue cache.  The routine
   works correctly for all n ≥ 1. */

#include <stdint.h>
#include <stdlib.h>

static idx_t cur_limit = 0;       /* current upper bound of the sieve                      */
static uint8_t *sieve = NULL;    /* 0 = prime, 1 = composite (size cur_limit+1)          */
static seq_t *residues = NULL;   /* cached residues for twin primes (size twin_count)     */
static size_t twin_count = 0;    /* number of residues cached                            */

/* ----------------------------------------------------------------------- */
/* Build a sieve up to @limit and compute the residues for all twin primes. */
static void rebuild(idx_t limit)
{
    /* free old sieve */
    free(sieve);
    sieve = (uint8_t*)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!sieve) exit(1);

    /* Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;
        }
    }

    /* count twin primes up to limit */
    size_t cnt = 0;
    for (idx_t i = 3; i + 2 <= limit; i += 2) {
        if (!sieve[i] && !sieve[i + 2])
            ++cnt;
    }

    /* allocate new residue array */
    seq_t *new_res = (seq_t*)malloc(cnt * sizeof(seq_t));
    if (!new_res) exit(1);

    /* fill residue array */
    size_t k = 0;
    for (idx_t i = 3; i + 2 <= limit; i += 2) {
        if (!sieve[i] && !sieve[i + 2]) {
            int r = (int)((i + 1) % 10);       /* 0 … 9 */
            if (r > 5)
                r -= 10;                       /* map to –5 … 5 */
            new_res[k++] = (seq_t)r;
        }
    }

    /* replace old cache */
    free(residues);
    residues = new_res;
    twin_count = cnt;
    cur_limit = limit;
}

/* ----------------------------------------------------------------------- */
/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1 – we ignore n==0 */
        return 0;

    /* Ensure cache contains at least n residues */
    if (residues == NULL || twin_count < (size_t)n) {
        idx_t limit = cur_limit;
        if (limit == 0) limit = 200000;      /* start with a modest limit */

        /* keep doubling the limit until we have enough twin primes */
        while (twin_count < (size_t)n) {
            rebuild(limit);
            if (twin_count >= (size_t)n)
                break;
            /* avoid overflow */
            if (limit > (UINT64_MAX >> 1))
                limit = UINT64_MAX;
            else
                limit *= 2;
        }
    }

    /* 1‑based index in residues array */
    return residues[n - 1];
}
