/*
 * A127931  Numbers k such that 13 divides 11*k + 2^k.
 * https://oeis.org/A127931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 5044, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A127931: Numbers k such that 13 divides 11*k + 2^k.
   Recurrence: a(n) = a(n-12) + 156 for n >= 13,
   with initial values
   a(1)=1, a(2)=2, a(3)=6, a(4)=9, a(5)=23, a(6)=29,
   a(7)=70, a(8)=72, a(9)=103, a(10)=112, a(11)=128, a(12)=147. */

seq_t a(idx_t n)
{
    /* Base values for n = 1..12 */
    static const seq_t base[12] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)6,
        (seq_t)9,
        (seq_t)23,
        (seq_t)29,
        (seq_t)70,
        (seq_t)72,
        (seq_t)103,
        (seq_t)112,
        (seq_t)128,
        (seq_t)147
    };

    /* Compute k = floor((n-1)/12) and idx = n - 12*k (1..12) */
    idx_t k   = (n - 1) / 12;
    idx_t idx = (n - 1) % 12 + 1;

    return base[idx - 1] + (seq_t)k * (seq_t)156;
}
