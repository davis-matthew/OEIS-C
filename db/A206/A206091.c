/*
 * A206091  Number of (n+1) X 6 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two counterclockwise and two clockwise edge
 *          increases.
 * https://oeis.org/A206091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 27, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206091
 * Formula: a(n) = 66 * a(n-3) for n > 8,
 * with a(1)=13608, a(2)=17976, a(3)=33792,
 * a(4)=131904, a(5)=516912, a(6)=2156544,
 * a(7)=8613504, a(8)=34039392. */
seq_t a(idx_t n)
{
    /* Base values for 1 ≤ n ≤ 8; index 0 unused */
    static const seq_t base[9] = {
        (seq_t)0,          /* base[0] unused */
        (seq_t)13608,
        (seq_t)17976,
        (seq_t)33792,
        (seq_t)131904,
        (seq_t)516912,
        (seq_t)2156544,
        (seq_t)8613504,
        (seq_t)34039392
    };

    /* For n ≤ 8 just return the stored value. */
    if (n <= 8) return base[n];

    /* Determine the appropriate base value for this modulus class. */
    idx_t mod = n % 3;
    seq_t init_val;
    idx_t base_idx;
    if (mod == 0) {
        base_idx = 6;
        init_val = base[6];
    } else if (mod == 1) {
        base_idx = 7;
        init_val = base[7];
    } else { /* mod == 2 */
        base_idx = 8;
        init_val = base[8];
    }

    /* Compute exponent e = (n - base_idx) / 3 and 66^e */
    idx_t exp = (n - base_idx) / 3;
    seq_t pow66 = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        pow66 *= (seq_t)66;
    }

    return init_val * pow66;
}
