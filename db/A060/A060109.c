/*
 * A060109  Numbers in Morse code, with 1 for a dot, 2 for a dash and 0
 *          between digits/letters.
 * https://oeis.org/A060109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is obtained by concatenating the Morse code representation
 *            (1 for dot, 2 for dash) of the decimal digits of n, with a 0
 *            separating the digits
 * Range:     u8 none, u16 n <= 9, u32 n <= 9, u64 n <= 999, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n-th term of OEIS sequence A060109. */
seq_t a(idx_t n)
{
    /* Morse code mappings: 1=dot, 2=dash.
       Each array contains the 5 digits for one decimal digit. */
    static const int map[10][5] = {
        {2,2,2,2,2}, /* 0 : -----   */
        {1,2,2,2,2}, /* 1 : .----   */
        {1,1,2,2,2}, /* 2 : ..---   */
        {1,1,1,2,2}, /* 3 : ...--   */
        {1,1,1,1,2}, /* 4 : ....-   */
        {1,1,1,1,1}, /* 5 : .....   */
        {2,1,1,1,1}, /* 6 : -....   */
        {2,2,1,1,1}, /* 7 : --...   */
        {2,2,2,1,1}, /* 8 : ---..   */
        {2,2,2,2,1}  /* 9 : ----.   */
    };

    /* Small buffer for decimal digits of n (enough for any idx_t). */
    int digits[32];
    int cnt = 0;

    /* Special case: n == 0 */
    if (n == 0) {
        seq_t res = 0;
        for (int k = 0; k < 5; ++k) {
            res = res * (seq_t)10 + (seq_t)map[0][k];
        }
        return res;
    }

    /* Extract decimal digits of n in reverse order */
    idx_t temp = n;
    while (temp > 0) {
        digits[cnt++] = (int)(temp % 10);
        temp /= 10;
    }

    /* Build the result by processing digits from most significant to least. */
    seq_t res = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        int d = digits[i];
        for (int k = 0; k < 5; ++k) {
            res = res * (seq_t)10 + (seq_t)map[d][k];
        }
        if (i > 0) {           /* add separator 0 between digits */
            res = res * (seq_t)10 + (seq_t)0;
        }
    }
    return res;
}
