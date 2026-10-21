/*
 * A239859  Number of 2 X n 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or zero plus the sum of the elements
 *          above it or one plus the sum of the elements diagonally to its
 *          northwest or one plus the sum of the elements antidiagonally to
 *          its northeast, modulo 4.
 * https://oeis.org/A239859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 65, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239859: Number of 2 × n 0..3 arrays satisfying the stated local rule. */
/* Recurrence (order 17) for n > 21:
   a(n) = a(n-1) + a(n-2) +7*a(n-3) -8*a(n-4) -7*a(n-5) +9*a(n-7) +10*a(n-8)
          -24*a(n-9) +5*a(n-10) +29*a(n-11) -18*a(n-12) -10*a(n-13) -10*a(n-14)
          +16*a(n-15) +12*a(n-16) -12*a(n-17)   */
#include <stdlib.h>

static const seq_t init[21] = {
    (seq_t)2,    /* a(1) */
    (seq_t)3,    /* a(2) */
    (seq_t)9,    /* a(3) */
    (seq_t)12,   /* a(4) */
    (seq_t)29,   /* a(5) */
    (seq_t)63,   /* a(6) */
    (seq_t)100,  /* a(7) */
    (seq_t)215,  /* a(8) */
    (seq_t)468,  /* a(9) */
    (seq_t)785,  /* a(10) */
    (seq_t)1654, /* a(11) */
    (seq_t)3515, /* a(12) */
    (seq_t)6048, /* a(13) */
    (seq_t)12676,/* a(14) */
    (seq_t)26381,/* a(15) */
    (seq_t)46460,/* a(16) */
    (seq_t)96810,/* a(17) */
    (seq_t)198286,/* a(18) */
    (seq_t)355994,/* a(19) */
    (seq_t)737992,/* a(20) */
    (seq_t)1493159 /* a(21) */
};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined, but return 0 for safety */
    if (n <= 21ULL) return init[n-1];

#if defined(__cplusplus)
    /* C++ version: use seq_t arithmetic directly (handles big‑ints). */
    seq_t buf[18];
    for (idx_t i = 1; i <= 21; ++i)
        buf[i % 18] = init[i-1];

    for (idx_t i = 22; i <= n; ++i) {
        int idx  = (int)(i % 18);
        int i1   = (int)((i-1) % 18);
        int i2   = (int)((i-2) % 18);
        int i3   = (int)((i-3) % 18);
        int i4   = (int)((i-4) % 18);
        int i5   = (int)((i-5) % 18);
        int i7   = (int)((i-7) % 18);
        int i8   = (int)((i-8) % 18);
        int i9   = (int)((i-9) % 18);
        int i10  = (int)((i-10) % 18);
        int i11  = (int)((i-11) % 18);
        int i12  = (int)((i-12) % 18);
        int i13  = (int)((i-13) % 18);
        int i14  = (int)((i-14) % 18);
        int i15  = (int)((i-15) % 18);
        int i16  = (int)((i-16) % 18);
        int i17  = (int)((i-17) % 18);

        seq_t sum_pos = buf[i1] + buf[i2]
                       + 7 * buf[i3]
                       + 9 * buf[i7]
                       +10 * buf[i8]
                       + 5 * buf[i10]
                       +29 * buf[i11]
                       +16 * buf[i15]
                       +12 * buf[i16];
        seq_t sum_neg =  8 * buf[i4]
                       + 7 * buf[i5]
                       +24 * buf[i9]
                       +18 * buf[i12]
                       +10 * buf[i13]
                       +10 * buf[i14]
                       +12 * buf[i17];
        buf[idx] = sum_pos - sum_neg;
    }
    return buf[n % 18];
#else
    /* C99 version: use unsigned __int128 for intermediate sums to avoid overflow. */
    typedef unsigned __int128 u128;
    seq_t buf[18];
    for (idx_t i = 1; i <= 21; ++i)
        buf[i % 18] = init[i-1];

    for (idx_t i = 22; i <= n; ++i) {
        int idx  = (int)(i % 18);
        int i1   = (int)((i-1) % 18);
        int i2   = (int)((i-2) % 18);
        int i3   = (int)((i-3) % 18);
        int i4   = (int)((i-4) % 18);
        int i5   = (int)((i-5) % 18);
        int i7   = (int)((i-7) % 18);
        int i8   = (int)((i-8) % 18);
        int i9   = (int)((i-9) % 18);
        int i10  = (int)((i-10) % 18);
        int i11  = (int)((i-11) % 18);
        int i12  = (int)((i-12) % 18);
        int i13  = (int)((i-13) % 18);
        int i14  = (int)((i-14) % 18);
        int i15  = (int)((i-15) % 18);
        int i16  = (int)((i-16) % 18);
        int i17  = (int)((i-17) % 18);

        u128 sum_pos = (u128)buf[i1] + (u128)buf[i2]
                     + 7 * (u128)buf[i3]
                     + 9 * (u128)buf[i7]
                     +10 * (u128)buf[i8]
                     + 5 * (u128)buf[i10]
                     +29 * (u128)buf[i11]
                     +16 * (u128)buf[i15]
                     +12 * (u128)buf[i16];
        u128 sum_neg =  8 * (u128)buf[i4]
                     + 7 * (u128)buf[i5]
                     +24 * (u128)buf[i9]
                     +18 * (u128)buf[i12]
                     +10 * (u128)buf[i13]
                     +10 * (u128)buf[i14]
                     +12 * (u128)buf[i17];
        u128 res = sum_pos - sum_neg;
        buf[idx] = (seq_t)res;
    }
    return buf[n % 18];
#endif
}
