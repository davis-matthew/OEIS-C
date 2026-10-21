/*
 * A093835  a(n) = n * Jacobsthal(n).
 * https://oeis.org/A093835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Jacobsthal(n) Jacobsthal numbers: J(0)=0, J(1)=1,
 *            J(n)=J(n-1)+2*J(n-2) for n≥2 a(n)=n*J(n)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* dynamic storage of Jacobsthal numbers, computed lazily */
static seq_t *J = NULL;    /* array of Jacobsthal numbers */
static idx_t  Jlen = 0;    /* number of elements currently stored (>=1) */

/* ensure that J[0..n] are available; returns 0 on success, non‑zero if malloc fails */
static int
ensure_jacobsthal(idx_t n)
{
    if (Jlen > n) return 0;          /* already available */

    /* allocate or grow array */
    size_t newlen = (size_t)(n + 1);
    seq_t *tmp = NULL;

    if (J == NULL) {
        tmp = (seq_t *)malloc(newlen * sizeof(seq_t));
        if (tmp == NULL) return -1;
    } else {
        tmp = (seq_t *)realloc(J, newlen * sizeof(seq_t));
        if (tmp == NULL) return -1;   /* keep old array on failure */
    }
    J = tmp;

    /* compute missing entries */
    for (idx_t i = Jlen; i <= n; ++i) {
        if (i == 0) {
            J[i] = (seq_t)0;
        } else if (i == 1) {
            J[i] = (seq_t)1;
        } else {
            J[i] = J[i-1] + J[i-2] + J[i-2];   /* J[i]=J[i-1]+2*J[i-2] */
        }
    }
    Jlen = (idx_t)newlen;
    return 0;
}

/* public function required by the tests */
seq_t a(idx_t n)
{
    if (ensure_jacobsthal(n) != 0) {
        /* on allocation failure the behaviour is undefined; return 0 */
        return (seq_t)0;
    }
    return ((seq_t)n) * J[n];
}
