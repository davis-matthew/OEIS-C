/*
 * A052057  Numbers k such that the decimal expansion of 2^k contains no
 *          palindromic substring except single digits.
 * https://oeis.org/A052057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A052057 (OEIS) – exponents k such that the decimal
 * expansion of 2^k contains no palindromic substring other than single
 * digits.  The function generates powers of 2 incrementally and
 * stores the good exponents in a static list.  It is O(number of
 * examined exponents) with simple decimal arithmetic. */

#include <stdint.h>

#define MAX_DIGITS   20000   /* enough for 2^50000 (≈15050 digits)   */
#define MAX_GOOD    20000   /* very generous upper bound            */

static int      goodk[MAX_GOOD];     /* list of valid exponents            */
static int      goodcnt = 0;         /* how many have been found so far     */
static uint8_t  digits[MAX_DIGITS];  /* little‑endian decimal digits of 2^k */
static int      len;                 /* number of digits currently stored   */
static uint64_t current_k = 0;       /* exponent of the current 2^k         */

/* Multiply current decimal number by 2 (little‑endian digits). */
static void mult_by_two(void)
{
    int carry = 0;
    for (int i = 0; i < len; ++i) {
        int v = digits[i] * 2 + carry;
        digits[i] = (uint8_t)(v % 10);
        carry = v / 10;
    }
    if (carry && len < MAX_DIGITS) {
        digits[len++] = (uint8_t)carry;
    }
}

/* Return 1 if any palindromic substring of length ≥ 2 exists. */
static int has_pal_substring(void)
{
    /* Length‑2 palindromes are just repeated adjacent digits. */
    for (int i = 0; i + 1 < len; ++i)
        if (digits[i] == digits[i + 1])
            return 1;

    /* Length‑3 or longer odd‑length palindromes. */
    for (int center = 0; center < len; ++center) {
        int l = center - 1;
        int r = center + 1;
        while (l >= 0 && r < len && digits[l] == digits[r]) {
            /* A match of any symmetry means a palindrome length ≥ 3. */
            return 1;
        }
    }

    /* Even‑length palindromes longer than 2 must have a middle pair of
     * identical digits, so they are already caught above by the
     * length‑2 check.  No further work is needed. */

    return 0;
}

/* Main sequence function.  The first call initializes the static state.
 * Subsequent calls expand the list until it contains at least n entries. */
seq_t a(idx_t n)
{
    if (goodcnt == 0) {          /* first call: set up 2^0 */
        len         = 1;
        digits[0]   = 1;         /* 2^0 = 1 */
        current_k   = 0;
        goodk[0]    = 0;         /* a(1) = 0 */
        goodcnt     = 1;
    }

    while (goodcnt < (int)n) {
        ++current_k;             /* next exponent */
        mult_by_two();           /* compute 2^current_k */
        if (!has_pal_substring())
            goodk[goodcnt++] = current_k;
    }

    return (seq_t)goodk[n - 1];
}
