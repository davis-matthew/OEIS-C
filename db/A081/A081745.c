/*
 * A081745  a(n) = A080901(n) - n.
 * https://oeis.org/A081745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A080901(n) - n, periodic with period 168; A080901(1)=2,
 *            A080901(k)=A080901(k-1)-2 if k already occurs, else
 *            A080901(k-1)+5. Computed by simulating one period
 * Range:     i8 n <= 68, i16 n <= 68, i32 n <= 68, i64 n <= 68, i128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int A081745_tab[169];
static bool A081745_ready = false;

static void A081745_init(void)
{
    static bool present[4000];
    int i;
    for (i = 0; i < 4000; i++) present[i] = false;
    int prev = 2;
    A081745_tab[1] = 2 - 1;
    present[2] = true;
    for (i = 2; i <= 168; i++) {
        int v;
        if (present[i]) v = prev - 2;
        else v = prev + 5;
        if (v >= 0 && v < 4000) present[v] = true;
        prev = v;
        A081745_tab[i] = v - i;
    }
    A081745_ready = true;
}

seq_t a(idx_t n)
{
    if (!A081745_ready) A081745_init();
    int r = (int)((n - 1) % 168) + 1;
    return (seq_t)A081745_tab[r];
}
