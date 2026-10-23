/*
 * A015420  Inverse of 1411th cyclotomic polynomial.
 * https://oeis.org/A015420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The coefficients a(n) of 1/Φ_1411(x) are periodic with period 1411.
   From the explicit product (x^83‑1)(x^17‑1)/((x^1411‑1)(x‑1)) one obtains the
   simple description:
       a(n) = 1  for n mod 1411 ∈ [0,16]
       a(n) = 0  for n mod 1411 ∈ [17,82] ∪ [100,1400]
       a(n) = –1 for n mod 1411 ∈ [83,99]
   This pattern is used to answer all queries. */

#include <stdbool.h>

static seq_t a_vals[1411];
static bool a_done = false;

static void a_init(void)
{
    if (a_done)
        return;
    for (idx_t i = 0; i < 1411; ++i) {
        if (i < 17)
            a_vals[i] = 1;
        else if (i < 83)
            a_vals[i] = 0;
        else if (i < 100)
            a_vals[i] = -1;
        else
            a_vals[i] = 0;
    }
    a_done = true;
}

seq_t a(idx_t n)
{
    a_init();
    return a_vals[n % 1411];
}
