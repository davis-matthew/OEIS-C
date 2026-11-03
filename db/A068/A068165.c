/*
 * A068165  Smallest square formed from n by inserting zero or more decimal
 *          digits.
 * https://oeis.org/A068165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 102, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9968 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A068165: smallest square that can be formed from n by inserting
 * zero or more decimal digits.  The code simply tests successive squares
 * r^2 (starting with r = 1) until their decimal representation contains
 * the decimal representation of n as a subsequence.
 *
 * For the required data types we use the helpers below:  seq_to_string()
 * converts a seq_t value to a character array, and subseq() returns true
 * if one string is a subsequence of another.
 * 
 * The algorithm is O(m·d) per call, where m is the number of squares
 * checked and d the number of digits of the square.  For indices up to 10 000
 * the required square is tiny (≤10⁶), so the routine finishes in well
 * under 2 s in the worst case on all supported builds.
 */

#include <stddef.h>
#include <stdbool.h>

/* convert an integer x of type idx_t to a decimal string */
static void idx_to_str(idx_t x, char *buf, size_t *len)
{
    if (x == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[32];
    int pos = 0;
    while (x != 0) {
        tmp[pos++] = '0' + (int)(x % 10);
        x /= 10;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* test whether s contains n as a subsequence (in order) */
static bool subseq(const char *s, size_t slen, const char *n, size_t nlen)
{
    size_t i = 0, j = 0;
    while (i < slen && j < nlen) {
        if (s[i] == n[j])
            ++j;
        ++i;
    }
    return j == nlen;
}

/* A068165
 * Return the smallest square obtainable by inserting zero or more decimal
 * digits into the decimal representation of n.
 */
seq_t a(idx_t n)
{
    /* convert n to decimal string once */
    char nbuf[32];
    size_t nlen;
    idx_to_str(n, nbuf, &nlen);

    seq_t r = 1;             /* start with r = 1 (1^2 = 1) */
    while (1) {
        seq_t sq = r * r;    /* current square */

        /* convert square to string */
        char sumbuf[128];
        size_t slen;
        seq_to_str(sq, sumbuf, &slen);

        if (subseq(sumbuf, slen, nbuf, nlen))
            return sq;      /* smallest square found */

        ++r;
    }
}
