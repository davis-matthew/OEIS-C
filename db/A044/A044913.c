/*
 * A044913  Numbers whose base-12 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑12 run lengths alternate odd, even, odd,
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates terms by brute‑force enumeration and memoisation. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: test whether a given integer satisfies the alternating run rule */
static bool is_valid(idx_t x)
{
    seq_t val = (seq_t)x;                     /* cast to seq_t for arithmetic   */
    seq_t temp = val;
    if (temp == 0) return false;              /* zero is not considered         */

    /* Extract base‑12 digits (least significant first) */
    int digits[64];
    int len = 0;
    while (temp > 0) {
        digits[len++] = (int)(temp % 12);
        temp /= 12;
    }

    /* Reverse to most significant first */
    for (int i = 0; i < len / 2; ++i) {
        int t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    bool expect_odd = true;   /* first run must be odd   */
    int runlen = 1;
    int prev = digits[0];

    for (int i = 1; i < len; ++i) {
        if (digits[i] == prev) {
            runlen++;
        } else {
            /* check parity of the completed run */
            if ((runlen & 1) != (expect_odd ? 1 : 0))
                return false;
            expect_odd = !expect_odd;     /* alternate expectation */
            prev = digits[i];
            runlen = 1;
        }
    }
    /* final run */
    if ((runlen & 1) != (expect_odd ? 1 : 0))
        return false;

    return true;
}

/* Cached list of found numbers.  The implementation uses a dynamic
   array that is grown with realloc when compiling as C and with
   new/delete when compiling as C++. */
static seq_t *valid_nums = NULL;
static size_t valid_cap = 0;   /* allocated capacity                 */
static size_t valid_len = 0;   /* number of elements currently stored */

/* Ensure there is enough capacity for 'new_cap' elements.  The function
   uses the appropriate allocation mechanism depending on the compiler. */
static void grow_to(size_t new_cap)
{
#if defined(__cplusplus)
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < valid_len; ++i)
        tmp[i] = valid_nums[i];
    if (valid_nums)
        delete[] valid_nums;
    valid_nums = tmp;
#else
    seq_t *tmp = realloc(valid_nums, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                /* out of memory */
    valid_nums = tmp;
#endif
    valid_cap = new_cap;
}

/* the required OEIS sequence generator */
seq_t a(idx_t n)
{
    /* generate terms until we have at least n of them */
    static idx_t next_checked = 0;   /* smallest integer still unchecked   */
    while (valid_len < n) {
        ++next_checked;
        if (is_valid(next_checked)) {
            if (valid_len == valid_cap)
                grow_to(valid_cap ? valid_cap * 2 : 16);
            valid_nums[valid_len++] = (seq_t)next_checked;
        }
    }
    return valid_nums[n - 1];
}
