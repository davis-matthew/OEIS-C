/*
 * A231117  Number of positive integers <= n and relatively prime to n which
 *          are squarefree if and only if n is squarefree.
 * https://oeis.org/A231117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A073311(n) if n is squarefree, else a(n) = phi(n) –
 *            A073311(n). The function is computed by direct counting of all
 *            numbers i ≤ n that are coprime to n and whose squarefree status
 *            matches that of n
 * Range:     u8 n <= 420, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: greatest common divisor for uint64_t */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Sieve that marks each integer ≤ limit as squarefree (true) or not (false). */
static bool *sqfree = NULL;
static idx_t sqfree_size = 0;

static void init_squarefree(idx_t limit)
{
    sqfree = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!sqfree) exit(1);
    for (idx_t i = 0; i <= limit; ++i) sqfree[i] = true;
    if (limit >= 0) sqfree[0] = false; /* 0 is not used, but mark */
    for (idx_t p = 2; (p * p) <= limit; ++p) {
        idx_t sq = p * p;
        for (idx_t j = sq; j <= limit; j += sq) {
            sqfree[j] = false;
        }
    }
    sqfree_size = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n=0, return 0 */
    if (n > sqfree_size) init_squarefree(n);

    bool n_sf = sqfree[n];              /* squarefree status of n */
    seq_t cnt = 0;

    for (idx_t i = 1; i <= n; ++i) {
        if (gcd_uint64(i, n) == 1) {    /* i is coprime to n */
            if (sqfree[i] == n_sf)      /* matches squarefree status */
                ++cnt;
        }
    }
    return cnt;
}
