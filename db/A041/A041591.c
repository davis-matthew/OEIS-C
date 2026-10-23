/*
 * A041591  Denominators of continued fraction convergents to sqrt(313).
 * https://oeis.org/A041591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 253724736*a(n-17) + a(n-34) for n > 33 Initial values:
 *            a(0)=1, a(1)=1, a(2)=3, a(3)=13, a(4)=146, a(5)=159, a(6)=305,
 *            a(7)=1074, a(8)=2453, a(9)=5980, a(10)=20393, a(11)=26373,
 *            a(12)=46766, a(13)=540799, a(14)=2209962, a(15)=4960723,
 *            a(16)=7170685 (sequence A041591: denominators of the con...
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 20, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic partial quotients of √313 */
    static seq_t partial[128];
    static idx_t period = 0;          /* number of terms after a0 */
    static int  initialized = 0;

    if (!initialized) {
        const idx_t D   = 313u;      /* the radicand */
        const idx_t a0  = 17u;       /* floor(√313) */
        idx_t m = 0;
        idx_t d  = 1;
        idx_t a  = a0;

        partial[0] = (seq_t)a0;      /* a0 at index 0 */
        period = 0;                  /* start counting periodic part */

        /* Compute the continued‑fraction period */
        while (1) {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            ++period;                /* store after a0 */
            partial[period] = (seq_t)a;
            if (a == 2 * a0)          /* last term of the period */
                break;
        }

        initialized = 1;
    }

    /* Denominator recurrence: q[-2] = 1, q[-1] = 0, q[k] = a[k]*q[k-1]+q[k-2] */
    seq_t q_nm2 = 1;   /* q[-2] */
    seq_t q_nm1 = 0;   /* q[-1] */
    seq_t q_k   = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0) {
            a_k = partial[0];
        } else {
            a_k = partial[1 + ((k - 1) % period)];
        }
        q_k = a_k * q_nm1 + q_nm2;
        q_nm2 = q_nm1;
        q_nm1 = q_k;
    }
    return q_k;
}
