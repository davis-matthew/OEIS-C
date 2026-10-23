/*
 * A169648  Number of cells that are turned from OFF to ON at stage n in
 *          Wolfram's 2-D cellular automaton defined by Rule 942.
 * https://oeis.org/A169648
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is given recursively by the following rules (OEIS A169648):
 *            – a(-1) = 0 – a(0) = 1, a(1) = 4, a(2) = 4, a(3) = 12, – a(4) =
 *            8, a(5) = 12, a(6) = 12, a(7) = 36 – for n ≥ 8, let n = 2^k + j
 *            with 0 ≤ j < 2^k * if j = 0: a(2^k) = 3 a(2^(k-1)) + 3·2^(k-1) –
 *            8 * if j = 2^(k-1): a(3·2^(k-1)) = 3 a(...
 * Range:     u8 n <= 30, u16 n <= 549, u32 n <= 549, u64 n <= 549,
 *            u128 n <= 549, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

extern seq_t a(idx_t n);

static const seq_t base[9] = {
    0, /* a(-1) */
    1, /* a(0) */
    4, /* a(1) */
    4, /* a(2) */
    12,/* a(3) */
    8, /* a(4) */
    12,/* a(5) */
    12,/* a(6) */
    36 /* a(7) */
};

seq_t a(idx_t n)
{
    if (n == -1) return base[0];
    if (n <= 7)  return base[n + 1];

    /* Find k such that 2^k <= n < 2^(k+1) */
    idx_t pow2k = 1;
    idx_t k = 0;
    while ((pow2k << 1) <= (uint64_t)n) {
        pow2k <<= 1;
        ++k;
    }

    idx_t j = n - pow2k;
    idx_t half = pow2k >> 1;      /* 2^(k-1) */
    idx_t quarter = pow2k >> 2;   /* 2^(k-2) */

    if (j == 0) {                         /* a(2^k) */
        seq_t a_half = a(half);
        return 3 * a_half + 3 * (seq_t)half - 8;
    } else if (j == half) {                /* a(3·2^(k-1)) */
        seq_t a_third = a(3 * quarter);
        return 3 * a_third + (seq_t)(pow2k << 1) - 24;
    } else {
        idx_t new_n = half + j;
        seq_t val = a(new_n);
        if (j > half)
            return 3 * val; /* a(2^k + j) = 3·a(2^(k-1)+j) */
        else
            return val;       /* a(2^k + j) = a(2^(k-1)+j) */
    }
}
