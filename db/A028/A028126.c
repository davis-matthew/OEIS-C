/*
 * A028126  Expansion of 1/((1-4x)(1-5x)(1-9x)(1-12x)).
 * https://oeis.org/A028126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=30, a(2)=583, a(3)=9372,
   a(n)=30*a(n-1)-317*a(n-2)+1392*a(n-3)-2160*a(n-4) for n≥4 */
seq_t a(idx_t n)
{
    /* Handle the first few terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)30;
    if (n == 2) return (seq_t)583;
    if (n == 3) return (seq_t)9372;

    /* Iterate the recurrence up to the requested n */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)30;  /* a(n-3) */
    seq_t a2 = (seq_t)583; /* a(n-2) */
    seq_t a3 = (seq_t)9372;/* a(n-1) */
    seq_t anum;

    for (idx_t i = 4; i <= n; ++i) {
        anum = 30 * a3;  /* 30*a(n-1) */
        anum -= 317 * a2;/* -317*a(n-2) */
        anum += 1392 * a1;/* +1392*a(n-3) */
        anum -= 2160 * a0;/* -2160*a(n-4) */

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = anum;
    }

    return a3;
}
