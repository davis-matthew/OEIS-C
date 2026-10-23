/*
 * A106202  Expansion of Im(x/(1 - x - 2*i*x^2)), i=sqrt(-1).
 * https://oeis.org/A106202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 37, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) - 4*a(n-4) for n ≥ 4
   with a(0)=a(1)=a(2)=0 and a(3)=2.
   This follows from the generating function 2*x^3/(1-2*x+x^2+4*x^4). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *tab = NULL;
    static idx_t size = 0;

    if (n < size)
        return tab[n];

    idx_t need = n + 1;
    seq_t *tmp = (seq_t *)realloc(tab, need * sizeof(seq_t));
    if (!tmp)
        return 0;   /* allocation failed; return 0 as safe fallback */

    tab = tmp;

    for (idx_t i = size; i < need; ++i) {
        if (i == 0 || i == 1 || i == 2) {
            tab[i] = 0;
        } else if (i == 3) {
            tab[i] = 2;
        } else {
            tab[i] = (seq_t)2 * tab[i - 1] - tab[i - 2] - (seq_t)4 * tab[i - 4];
        }
    }

    size = need;
    return tab[n];
}
