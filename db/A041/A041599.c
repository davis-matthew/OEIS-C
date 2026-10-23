/*
 * A041599  Denominators of continued fraction convergents to sqrt(317).
 * https://oeis.org/A041599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041599: denominators of continued fraction convergents to sqrt(317)
   Recurrence: a(n) = 705236*a(n-11) + a(n-22)  for n > 21
   Initial values:
   a(0)=1, a(1)=1, a(2)=5, a(3)=41, a(4)=46, a(5)=133, a(6)=312,
   a(7)=445, a(8)=3872, a(9)=15933, a(10)=19805, a(11)=689303,
   a(12)=709108, a(13)=3525735, a(14)=28914988, a(15)=32440723,
   a(16)=93796434, a(17)=220033591, a(18)=313830025,
   a(19)=2730673791, a(20)=11236525189, a(21)=13967198980 */

#include <stddef.h>

static void compute_base(seq_t *base)
{
    const int D = 317;
    const int a0 = 17;
    unsigned long long m = 0;
    unsigned long long d = 1;
    unsigned long long a = a0;

    seq_t p_nm2 = (seq_t)0;
    seq_t p_nm1 = (seq_t)1;
    seq_t q_nm2 = (seq_t)1;
    seq_t q_nm1 = (seq_t)0;

    for (int k = 0; k < 22; ++k)
    {
        seq_t a_seq = (seq_t)a;

        seq_t p_n = a_seq * p_nm1 + p_nm2;
        seq_t q_n = a_seq * q_nm1 + q_nm2;
        base[k] = q_n;

        p_nm2 = p_nm1;
        p_nm1 = p_n;
        q_nm2 = q_nm1;
        q_nm1 = q_n;

        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
    }
}

seq_t a(idx_t n)
{
    static seq_t base[22];
    static int initialized = 0;

    if (!initialized)
    {
        compute_base(base);
        initialized = 1;
    }

    if (n < 22)
        return base[(size_t)n];

    seq_t buf[22];
    for (size_t i = 0; i < 22; ++i)
        buf[i] = base[i];

    for (idx_t i = 22; i <= n; ++i)
    {
        seq_t term = (seq_t)705236 * buf[(i - 11) % 22] + buf[(i - 22) % 22];
        buf[i % 22] = term;
    }

    return buf[n % 22];
}
