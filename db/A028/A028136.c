/*
 * A028136  Expansion of 1/((1-4x)(1-6x)(1-8x)(1-10x)).
 * https://oeis.org/A028136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=28, a(2)=500, a(3)=7280,
   a(n)=28*a(n-1)-284*a(n-2)+1232*a(n-3)-1920*a(n-4) */

static inline seq_t mul_const(const seq_t a, unsigned int c)
{
#if defined(__SIZEOF_INT128__)
    if (sizeof(seq_t) <= 8)
        return (seq_t)((unsigned __int128)a * (unsigned __int128)c);
#endif
    return a * (seq_t)c;
}

seq_t a(idx_t n)
{
    /* Initial terms */
    static const seq_t init[4] = { (seq_t)1, (seq_t)28, (seq_t)500, (seq_t)7280 };

    if (n < 4)
        return init[n];

    /* a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = mul_const(a3, 28);
        next -= mul_const(a2, 284);
        next += mul_const(a1, 1232);
        next -= mul_const(a0, 1920);

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
