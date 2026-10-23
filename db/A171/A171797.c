/*
 * A171797  A modified Sisyphus function: a(n) = concatenation of (number of
 *          digits in n) (number of even digits) (number of odd digits).
 * https://oeis.org/A171797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = concatenation of (number of digits in n) (number of even
 *            digits) (number of odd digits)
 * Range:     u8 n <= 99, u16 n <= 9255, u32 n <= 9255, u64 n <= 9255,
 *            u128 n <= 9255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int seq_digit_len(seq_t x)
{
    if (x == (seq_t)0)
        return 1;
    int len = 0;
    while (x != (seq_t)0) {
        ++len;
        x /= (seq_t)10;
    }
    return len;
}

static seq_t seq_pow10(idx_t e)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < e; ++i)
        r *= (seq_t)10;
    return r;
}

seq_t a(idx_t n)
{
    idx_t tmp = n;
    int d = 0;   /* number of digits of n */
    int e = 0;   /* number of even digits in n */
    int o = 0;   /* number of odd  digits in n */

    if (tmp == 0) {
        d = 1;      /* '0' has one digit */
        e = 1;      /* digit 0 is even */
        o = 0;
    } else {
        while (tmp > 0) {
            int digit = tmp % 10;
            ++d;
            if ((digit & 1) == 0)
                ++e;
            else
                ++o;
            tmp /= 10;
        }
    }

    int len_e = seq_digit_len((seq_t)e);
    int len_o = seq_digit_len((seq_t)o);

    seq_t res = ((seq_t)d) * seq_pow10((idx_t)(len_e + len_o))
              + ((seq_t)e) * seq_pow10((idx_t)len_o)
              + (seq_t)o;

    return res;
}
