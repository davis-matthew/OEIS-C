/*
 * A106789  Sum of two consecutive squares of Lucas 3-step numbers (A001644).
 * https://oeis.org/A106789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A001644(n)^2 + A001644(n+1)^2 where A001644 is the Lucas
 *            3‑step numbers: A001644(0)=3, A001644(1)=1, A001644(2)=3,
 *            A001644(k)=A001644(k-1)+A001644(k-2)+A001644(k-3) for k≥3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *lucas = NULL;
static idx_t lucas_size = 0;   /* number of elements currently stored (indices 0..size-1) */
static idx_t lucas_cap  = 0;   /* allocated capacity */

/* ensure that lucas[n] is defined for all n ≤ req */
static void ensure_lucas(idx_t req)
{
    if (lucas_size > req) return;  /* all needed elements already present */

    /* initialise first three terms if this is the first call */
    if (lucas_size == 0) {
        if (lucas_cap < 4) {
            lucas_cap = 4;
            lucas = (seq_t *)malloc(lucas_cap * sizeof(seq_t));
        }
        lucas[0] = (seq_t)3;
        lucas[1] = (seq_t)1;
        lucas[2] = (seq_t)3;
        lucas_size = 3;
    }

    /* extend capacity as needed */
    if (lucas_cap <= lucas_size) {
        size_t new_cap = lucas_cap * 2;
        if (new_cap < lucas_size + 1) new_cap = lucas_size + 1;
        lucas = (seq_t *)realloc(lucas, new_cap * sizeof(seq_t));
        lucas_cap = new_cap;
    }

    /* compute further terms */
    while (lucas_size <= req) {
        seq_t v = lucas[lucas_size-1] + lucas[lucas_size-2] + lucas[lucas_size-3];
        lucas[lucas_size++] = v;
    }
}

seq_t a(idx_t n)
{
    /* we need terms lucas[n] and lucas[n+1] */
    ensure_lucas(n + 1);

    seq_t x = lucas[n];
    seq_t y = lucas[n+1];
    return x * x + y * y;
}
