/*
 * A100961  For a decimal string s, let f(s) = decimal string ijk, where i =
 *          number of even digits in s, j = number of odd digits in s, k=i+j
 *          (see A171797). Start with s = decimal expansion of n; a(n) =
 *          number of applications of f needed to reach the string 123.
 * https://oeis.org/A100961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of applications of f needed to reach the string
 *            123 * starting from the decimal representation of n. * Algorithm
 *            simulates f(s) until s == "123". The function works for any * n
 *            ≥ 0; it does not rely on any special sequence data
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* working buffer for the string representation of the current state */
    char s[20];
    int   len = 0;

    /* convert the initial n to a decimal string in s */
    if (n == 0) {
        s[0] = '0';
        len = 1;
    } else {
        idx_t tmp = n;
        char  rev[20];
        int   r = 0;
        while (tmp > 0) {
            rev[r++] = (char)('0' + (tmp % 10));
            tmp /= 10;
        }
        for (int i = r - 1, j = 0; i >= 0; --i, ++j) {
            s[j] = rev[i];
        }
        len = r;
    }

    seq_t steps = 0;

    /* iterate until the string equals "123" */
    while (!(len == 3 && s[0] == '1' && s[1] == '2' && s[2] == '3')) {
        int ev = 0, od = 0;
        for (int i = 0; i < len; ++i) {
            /* digit value */
            int d = s[i] - '0';
            /* check parity using bitwise operation */
            if ((d & 1) == 0)
                ++ev;
            else
                ++od;
        }
        int k = ev + od;

        /* construct the new string i j k */
        s[0] = (char)('0' + ev);
        s[1] = (char)('0' + od);
        s[2] = (char)('0' + k);
        len = 3;

        ++steps;
    }

    return steps;
}
