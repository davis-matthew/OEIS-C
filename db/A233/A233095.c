/*
 * A233095  Number of nX5 0..3 arrays with no element x(i,j) adjacent to value
 *          3-x(i,j) horizontally, diagonally or antidiagonally, top left
 *          element zero, and 1 appearing before 2 in row major order.
 * https://oeis.org/A233095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 79*a(n-1) -1121*a(n-2) +3055*a(n-3) +8224*a(n-4)
 *            -49962*a(n-5) +81450*a(n-6) -48132*a(n-7) -2272*a(n-8)
 *            +11272*a(n-9) -2688*a(n-10) +96*a(n-11); initial values
 *            (a(1)…a(11)): 41, 2315, 141566, 8714705, 537122150, 33113065637,
 *            2041493495546, 125863931140721, 7759890074654654,
 *            478420800866866...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* needed for bool */
#include <stdbool.h>

/* convert decimal string to seq_t */
static inline seq_t parse_decimal(const char *s)
{
    seq_t r = 0;
    while (*s) {
        r = r * 10 + (seq_t)(*s - '0');
        ++s;
    }
    return r;
}

/* string representations of the first 11 terms */
static const char *const init_str[11] = {
    "41",
    "2315",
    "141566",
    "8714705",
    "537122150",
    "33113065637",
    "2041493495546",
    "125863931140721",
    "7759890074654654",
    "478420800866866973",
    "29496098980798824098"
};

seq_t a(idx_t n)
{
    /* lazily convert the initial terms once */
    static bool init_done = false;
    static seq_t init[11];
    if (!init_done) {
        for (int i = 0; i < 11; ++i)
            init[i] = parse_decimal(init_str[i]);
        init_done = true;
    }

    if (n == 0) return (seq_t)0;          /* out of offset – undefined */
    if (n <= 11) return init[n - 1];

    /* buffer holds last 11 terms:
     * buf[0] = a(k-1), buf[1] = a(k-2), … , buf[10] = a(k-11)   */
    seq_t buf[11];
    for (int i = 0; i < 11; ++i)
        buf[i] = init[10 - i];        /* reverse order: buf[0] = a(11) … buf[10] = a(1) */

    for (idx_t cur = 12; cur <= n; ++cur) {
        seq_t pos = 0, sub = 0;
        pos += (seq_t)79    * buf[0];
        pos += (seq_t)3055  * buf[2];
        pos += (seq_t)8224  * buf[3];
        pos += (seq_t)81450 * buf[5];
        pos += (seq_t)11272 * buf[8];
        pos += (seq_t)96    * buf[10];

        sub += (seq_t)1121  * buf[1];
        sub += (seq_t)49962 * buf[4];
        sub += (seq_t)48132 * buf[6];
        sub += (seq_t)2272  * buf[7];
        sub += (seq_t)2688  * buf[9];

        seq_t term = pos - sub;       /* recurrence */

        /* shift buffer and insert new term */
        for (int i = 10; i > 0; --i) buf[i] = buf[i - 1];
        buf[0] = term;
    }
    return buf[0];
}
