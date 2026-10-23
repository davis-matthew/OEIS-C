/*
 * A104406  Number of numbers <= n having no 2 in ternary representation.
 * https://oeis.org/A104406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   count of numbers ≤ n that contain no digit 2 in their base‑3
 *            representation
 * Range:     u8 n <= 3556, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* early exit for n = 0 (outside the offset) */
    if (n == 0)
        return 0;

    /* extract ternary digits of n, least significant first */
    int d[70];
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        d[len++] = (int)(tmp % 3);
        tmp /= 3;
    }
    /* reverse to most significant first */
    for (int i = 0; i < len / 2; ++i) {
        int t = d[i];
        d[i]  = d[len - 1 - i];
        d[len - 1 - i] = t;
    }

    /* pre‑compute powers of 2 up to 'len' (cached across calls) */
    static seq_t pow2[70] = {0};
    static int pow2_len  = 0;
    if (pow2_len == 0) {
        pow2[0] = 1;
        pow2_len = 1;
    }
    if (pow2_len <= len) {
        for (int i = pow2_len; i <= len; ++i)
            pow2[i] = pow2[i - 1] * ((seq_t)2);
        pow2_len = len + 1;
    }

    seq_t ans = 0;
    int has_two = 0;

    for (int pos = 0; pos < len; ++pos) {
        int digit = d[pos];
        int rem   = len - pos - 1;          /* remaining lower positions   */
        if (digit == 0) {
            /* must match 0; nothing added */
            continue;
        } else if (digit == 1) {
            ans += pow2[rem];               /* choose 0 here; rest free   */
            /* continue with the same prefix */
        } else { /* digit == 2 */
            has_two = 1;
            ans += ((seq_t)2) * pow2[rem];  /* choose 0 or 1; all lower free */
            break;                          /* cannot match 2 with allowed digits */
        }
    }

    if (!has_two)
        ans += 1;  /* n itself is valid and was not counted yet */

    /* exclude 0 from the count */
    if (ans > 0)
        ans -= 1;

    return ans;
}
