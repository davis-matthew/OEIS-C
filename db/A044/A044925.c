/*
 * A044925  a(n)=so-se, where so(se)=sum of odd(even) base 3 run lengths of n.
 * https://oeis.org/A044925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = ∑_{runs in base‑3 repr of n} ( +len if len odd, -len if
 *            len even )
 * Range:     i8 n <= 82, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. add each odd run length and subtract each even run length. */
seq_t a(idx_t n) {
    /* For n = 0 the sequence is undefined; we return 0 for safety. */
    if (n == 0) return 0;

    /* base‑3 digits of n, stored least significant first */
    int digits[64];
    int len = 0;
    idx_t t = n;
    while (t > 0) {
        digits[len++] = t % 3;
        t /= 3;
    }

    /* Scan runs from most significant to least significant digit */
    seq_t sum = 0;
    int cur_digit = digits[len - 1];
    int run_len = 1;
    for (int i = len - 2; i >= 0; --i) {
        int d = digits[i];
        if (d == cur_digit) {
            ++run_len;
        } else {
            if (run_len & 1)      /* odd length: add */
                sum += (seq_t)run_len;
            else                 /* even length: subtract */
                sum -= (seq_t)run_len;
            cur_digit = d;
            run_len = 1;
        }
    }
    /* last run */
    if (run_len & 1)
        sum += (seq_t)run_len;
    else
        sum -= (seq_t)run_len;

    return sum;
}
