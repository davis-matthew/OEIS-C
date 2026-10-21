/*
 * A071228  a(n) = n*(n-th composite number).
 * https://oeis.org/A071228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A002808(n), n ≥ 1 (A002808 = the n‑th composite
 *            number)
 * Range:     u8 n <= 12, u16 n <= 226, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*---------- local state for composite number generation ----------*/
static uint64_t *cached_composites = NULL;   /* array of found composite numbers */
static idx_t   cached_capacity = 0;           /* current allocated size of the array */
static idx_t   cached_size     = 0;           /* number of composites stored */
static idx_t   next_candidate = 4;             /* next integer to test for compositeness */

/*---------- very small deterministic primality test (idx_t ≤ 2³⁶⁴) ----------*/
static int is_prime(idx_t n)
{
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if ((n & 1) == 0) return 0;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return 0;
    return 1;
}

/*---------- extend the cache so that it contains at least 'n' composites ----------*/
static void ensure_composites(idx_t n)
{
    if (cached_size >= n) return;

    /* grow array if needed */
    if (cached_capacity < n) {
        size_t new_cap = cached_capacity ? cached_capacity * 2u : 16u;
        while (new_cap < n) new_cap <<= 1u;
        uint64_t *tmp = (uint64_t *)realloc(cached_composites, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);          /* unrecoverable memory failure */
        cached_composites = tmp;
        cached_capacity   = new_cap;
    }

    /* fill the cache up to the required size */
    while (cached_size < n) {
        if (!is_prime(next_candidate))
            cached_composites[cached_size++] = next_candidate;
        ++next_candidate;
    }
}

/*---------- the required sequence function ----------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safe guard – offset 1 is the first real term */
    ensure_composites(n);
    seq_t n_seq   = (seq_t)n;
    seq_t comp_seq = (seq_t)cached_composites[n - 1];
    return n_seq * comp_seq;
}
