/*
 * A204908  Least k such that n divides s(k)-s(j) for some j in [1,k], where
 *          s(k) is the k-th prime >=5.
 * https://oeis.org/A204908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 166, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A204908:
 *  a(n) = least k such that there exist j < k with s(k) ≡ s(j) (mod n),
 *  where s(k) is the k‑th prime ≥5.
 *  This is found by scanning the primes modulo n until a residue repeats.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static uint64_t *prime_list = NULL;   /* primes ≥5, 0‑based index */
static size_t   prime_count = 0;      /* number of stored primes */

/* ensure the prime list contains enough primes for n up to 10000
 * (the first 20001 primes are sufficient since a(n) ≤ n+1). */
static void
init_primes(void)
{
    if (prime_list)
        return;                          /* already initialised */

    /* upper bound for the 20001‑st prime is 300000 (≈ 26300 primes below it) */
    const size_t LIMIT = 300000;
    bool *comp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!comp)
        exit(1);

    for (size_t p = 2; p * p <= LIMIT; ++p)
        if (!comp[p])
            for (size_t q = p * p; q <= LIMIT; q += p)
                comp[q] = true;

    /* count primes ≥5 */
    size_t cnt = 0;
    for (size_t i = 5; i <= LIMIT; ++i)
        if (!comp[i])
            ++cnt;

    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_list)
        exit(1);

    size_t idx = 0;
    for (size_t i = 5; i <= LIMIT; ++i)
        if (!comp[i])
            prime_list[idx++] = (uint64_t)i;

    prime_count = cnt;
    free(comp);
}

/* external linkage as required by the build */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    init_primes();

    /* the first repeat must occur within the first n+1 primes */
    size_t limit = (size_t)n + 1u;

    /* visited residues modulo n, 0..n-1 */
    char *seen = (char*)calloc((size_t)n, sizeof(char));
    if (!seen)
        exit(1);

    for (size_t k = 1; k <= limit; ++k) {
        seq_t r = (seq_t)prime_list[k - 1] % (seq_t)n;   /* residue */
        size_t idx = (size_t)r;                          /* idx < n */

        if (seen[idx]) {
            free(seen);
            return (seq_t)k;                            /* first duplicate found */
        }
        seen[idx] = 1U;
    }

    free(seen);
    return (seq_t)limit; /* fallback (should never happen) */
}
