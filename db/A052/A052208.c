/*
 * A052208  a(n) = Pell(n)*Pell(2*n)/2.
 * https://oeis.org/A052208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Pell(n) * Pell(2*n) / 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute Pell numbers P(0)=0, P(1)=1, P(k)=2*P(k-1)+P(k-2) */
/* The array pell[] stores Pell values up to the largest index needed. */

static seq_t *pell = NULL;      /* dynamic array of Pell numbers */
static size_t pell_len = 0;     /* number of elements currently stored (max index+1) */

/* Ensure that Pell numbers up to index idx are computed and stored. */
static void ensure_pell(size_t idx)
{
    if (pell_len > idx) return;          /* already computed */

    /* Resize the array to accommodate indices 0..idx inclusive. */
    seq_t *tmp = (seq_t *)realloc(pell, (idx + 1) * sizeof(seq_t));
    pell = tmp;
    if (pell == NULL) return;            /* allocation failed; will crash on use */

    /* Initialise base cases if they are not yet present. */
    if (pell_len == 0) {
        pell[0] = (seq_t)0;
        pell_len = 1;
    }
    if (pell_len == 1 && idx >= 1) {
        pell[1] = (seq_t)1;
        pell_len = 2;
    }

    /* Compute subsequent Pell numbers. */
    for (size_t i = pell_len; i <= idx; ++i) {
        pell[i] = pell[i-1] * ((seq_t)2) + pell[i-2];
    }
    pell_len = idx + 1;
}

seq_t a(idx_t n)
{
    /* Pell numbers needed up to index 2*n. */
    size_t required = (size_t)(2 * n);
    ensure_pell(required);

    seq_t p_n  = pell[(size_t)n];
    seq_t p_2n = pell[required];

    /* Divide first to avoid overflow: at least one of p_n or p_2n is even. */
    seq_t result;
    if ((p_2n % ((seq_t)2)) == 0) {
        result = (p_2n / ((seq_t)2)) * p_n;
    } else {
        result = (p_n / ((seq_t)2)) * p_2n;
    }
    return result;
}
