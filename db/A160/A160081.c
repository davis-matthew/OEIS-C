/*
 * A160081  Lodumo_5 of Fibonacci numbers.
 * https://oeis.org/A160081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A160081 – Lodumo_5 of Fibonacci numbers
 *
 *  Recurrence for n >= 15:
 *      a(n) = a(n‑1)+a(n‑2)-a(n‑3)-a(n‑4)+2*a(n‑5)
 *             -2*a(n‑7)+2*a(n‑9)-a(n‑10)-a(n‑11)+a(n‑12)+a(n‑13)-a(n‑14)
 *
 *  Initial values (n = 0 … 14):
 *      a(0)=0, a(1)=1, a(2)=6, a(3)=2, a(4)=3,
 *      a(5)=5, a(6)=8, a(7)=13, a(8)=11, a(9)=4,
 *      a(10)=10, a(11)=9, a(12)=14, a(13)=18, a(14)=7
 *
 *  Periodicity:
 *      a(n+20) = a(n) + 20   for all n >= 0
 */

#include <stddef.h>   /* for size_t (not used but harmless) */

/* Pre‑computed first 20 terms using the recurrence and the initial values */
static seq_t period[20];
static int period_inited = 0;

static void init_period(void)
{
    if (period_inited)
        return;

    /* Initial values n = 0 … 14 */
    period[0] = (seq_t)0;
    period[1] = (seq_t)1;
    period[2] = (seq_t)6;
    period[3] = (seq_t)2;
    period[4] = (seq_t)3;
    period[5] = (seq_t)5;
    period[6] = (seq_t)8;
    period[7] = (seq_t)13;
    period[8] = (seq_t)11;
    period[9] = (seq_t)4;
    period[10] = (seq_t)10;
    period[11] = (seq_t)9;
    period[12] = (seq_t)14;
    period[13] = (seq_t)18;
    period[14] = (seq_t)7;

    /* Compute n = 15 … 19 by the recurrence */
    for (idx_t i = 15; i < 20; ++i) {
        period[i] =
            period[i - 1] + period[i - 2]
            - period[i - 3] - period[i - 4]
            + (period[i - 5] << 1)                /* 2 * a(n-5) */
            - (period[i - 7] << 1)                /* -2 * a(n-7) */
            + (period[i - 9] << 1)                /* +2 * a(n-9) */
            - period[i -10] - period[i -11]
            + period[i -12] + period[i -13]
            - period[i -14];
    }
    period_inited = 1;
}

seq_t a(idx_t n)
{
    init_period();

    /* Use the periodic property a(n+20) = a(n) + 20 */
    idx_t q = n / 20;          /* quotient */
    idx_t r = n % 20;          /* remainder */

    /* Compute base value + 20 * quotient */
    seq_t result = period[r] + (seq_t)20 * (seq_t)q;
    return result;
}
