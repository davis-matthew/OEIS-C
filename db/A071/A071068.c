/*
 * A071068  Number of ways to write n as a sum of two unordered squarefree
 *          numbers.
 * https://oeis.org/A071068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = |{(k, n-k) : 1 ≤ k ≤ n/2, k and n-k are square‑free}|
 * Range:     u8 n <= 899, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function precomputes a sieve of square‑free numbers up to the largest n
   seen across calls and then counts unordered pairs. */
#include <stdlib.h>

static unsigned char *sqfree = NULL;   /* 1 if i is square‑free, 0 otherwise */
static idx_t sqfree_len = 0;           /* allocated length (indices 0 .. len-1) */

/* Ensure sqfree[0..upto] is correctly filled. */
static void ensure_squarefree(idx_t upto)
{
    if (upto + 1 <= sqfree_len) return;              /* already large enough */

    idx_t new_len = upto + 1;
    unsigned char *ptr = (unsigned char *)realloc(sqfree, (size_t)new_len);
    if (!ptr) abort();                              /* allocation failed */
    sqfree = ptr;

    /* Initialise newly added entries to 1 (square‑free). */
    for (idx_t i = sqfree_len; i < new_len; ++i) sqfree[i] = 1;
    if (new_len > 0) sqfree[0] = 0;
    if (new_len > 1) sqfree[1] = 1;

    /* Mark numbers that are divisible by a square > 1. */
    for (idx_t p = 2; p * p <= upto; ++p) {
        idx_t sq = p * p;
        for (idx_t j = sq; j <= upto; j += sq) sqfree[j] = 0;
    }

    sqfree_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    ensure_squarefree(n);

    seq_t count = 0;
    idx_t half = n / 2;
    for (idx_t k = 1; k <= half; ++k) {
        if (sqfree[k] && sqfree[n - k]) ++count;
    }
    return count;
}
