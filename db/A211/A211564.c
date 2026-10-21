/*
 * A211564  Number of nonnegative integer arrays of length n+4 with new values
 *          0 upwards introduced in order, and containing the value n-1.
 * https://oeis.org/A211564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) (n>9)
 *            Initial values: a(1)=52, a(2)=202, a(3)=813, a(4)=3046,
 *            a(5)=10096, a(6)=29503, a(7)=77078, a(8)=183074, a(9)=401337
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 32, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _a_init[9] = {
    (seq_t)52,   (seq_t)202,  (seq_t)813,  (seq_t)3046,
    (seq_t)10096,(seq_t)29503,(seq_t)77078,(seq_t)183074,
    (seq_t)401337
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n<1 */

    if (n <= 9) return _a_init[n-1];         /* return pre‑computed values */

    /* array holds the last nine terms:
       _buf[0] = a(i-1), _buf[1] = a(i-2), … , _buf[8] = a(i-9) */
    seq_t _buf[9];
    _buf[0] = _a_init[8];   /* a(9)   */
    _buf[1] = _a_init[7];   /* a(8)   */
    _buf[2] = _a_init[6];   /* a(7)   */
    _buf[3] = _a_init[5];   /* a(6)   */
    _buf[4] = _a_init[4];   /* a(5)   */
    _buf[5] = _a_init[3];   /* a(4)   */
    _buf[6] = _a_init[2];   /* a(3)   */
    _buf[7] = _a_init[1];   /* a(2)   */
    _buf[8] = _a_init[0];   /* a(1)   */

    for (idx_t i = 10; i <= n; ++i) {
        seq_t newterm =
            _buf[0]*(seq_t)9
          - _buf[1]*(seq_t)36
          + _buf[2]*(seq_t)84
          - _buf[3]*(seq_t)126
          + _buf[4]*(seq_t)126
          - _buf[5]*(seq_t)84
          + _buf[6]*(seq_t)36
          - _buf[7]*(seq_t)9
          + _buf[8];

        /* shift buffer right, insert new term at position 0 */
        for (idx_t k = 8; k > 0; --k) _buf[k] = _buf[k-1];
        _buf[0] = newterm;
    }

    return _buf[0];
}
