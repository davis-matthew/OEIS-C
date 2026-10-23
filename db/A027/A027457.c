/*
 * A027457  a(n) = (H(n) - 1)*lcm{1,...,n}, where H(n) is the n-th harmonic
 *          number.
 * https://oeis.org/A027457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (H(n)-1)*lcm{1,2,...,n}
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* H(n) is the nth harmonic number.  */

static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* a(1) = 0, lcm(1) = 1 */
    if (n < 1) return 0;              /* safety for n==0, though offset starts at 1 */

    seq_t lcm = 1;                     /* current lcm for 1..k-1 */
    seq_t val = 0;                     /* current a(k-1) */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;

        seq_t g      = seq_gcd(lcm, k_seq);         /* gcd(lcm, k) */
        seq_t ratio  = k_seq / g;                    /* lcm_{k}/lcm_{k-1} = k/g */
        seq_t lcm_div = lcm / g;                     /* lcm_{k-1} / g */

        /* a(k) = a(k-1)*ratio + lcm_{k-1}/g   (since lcm_{k-1}/g = lcm_div) */
        val = val * ratio + lcm_div;

        /* Update lcm for next iteration: lcm_{k} = lcm_{k-1}*ratio */
        lcm = lcm_div * k_seq;
    }

    return val;
}
