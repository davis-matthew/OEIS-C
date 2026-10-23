/*
 * A021644  Expansion of 1/((1-x)*(1-3*x)*(1-8*x)*(1-10*x)).
 * https://oeis.org/A021644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=22, a(2)=329, a(3)=4202
   a(n) = 22*a(n-1) - 155*a(n-2) + 374*a(n-3) - 240*a(n-4)   for n ≥ 4 */

static inline seq_t mul_const(seq_t x, unsigned int c)
{
    seq_t r = (seq_t)0;
    for (unsigned int i = 0; i < c; ++i)
        r += x;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;
    if (n == 2) return (seq_t)329;
    if (n == 3) return (seq_t)4202;

    seq_t a0 = (seq_t)1,  a1 = (seq_t)22,
          a2 = (seq_t)329, a3 = (seq_t)4202;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t term1 = mul_const(a3, 22);
        seq_t term2 = mul_const(a2, 155);
        seq_t term3 = mul_const(a1, 374);
        seq_t term4 = mul_const(a0, 240);

        seq_t an = term1 - term2 + term3 - term4;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
