/*
 * A043030  Base-9 palindromes that start with 3.
 * https://oeis.org/A043030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 101, u32 n <= 911, u64 n <= 911,
 *            u128 n <= 911, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration: base‑9 palindromes that start with 3 in increasing order. */
/* Each term is the decimal value of a base‑9 palindrome whose most‑significant digit is 3. */
/* Example: a(1)=3   (3_9), a(2)=30  (33_9), a(3)=246 (303_9), ... */

#include <stdint.h>
#include <stdbool.h>

/* Precompute powers of 9 up to a reasonable limit for the counts of
   free digits (halfLen-1).  We store them in idx_t to compare with the
   remaining index, which is a uint64_t. */
static idx_t _pow9[64];
static bool _pow9_inited = false;

static void _init_pow9(void)
{
    if (_pow9_inited) return;
    _pow9[0] = 1;
    for (int i = 1; i < 64; ++i) {
        if (_pow9[i-1] > (idx_t)(-1) / 9)
            _pow9[i] = (idx_t)(-1);   /* saturate on overflow */
        else
            _pow9[i] = _pow9[i-1] * 9;
    }
    _pow9_inited = true;
}

seq_t a(idx_t n)
{
    if (! _pow9_inited) _init_pow9();
    if (n == 0) return 0;   /* undefined for n=0, return 0 for safety */

    idx_t remaining = n - 1;   /* 0‑based index among all generated terms */

    int L = 1;                 /* current palindrome length */
    while (1) {
        int halfLen = (L + 1) / 2;          /* ceil(L/2) */
        idx_t count  = _pow9[halfLen - 1];   /* 9^(halfLen-1) palindromes with this length */
        if (remaining < count) break;
        remaining -= count;
        ++L;
    }

    int halfLen = (L + 1) / 2;
    uint8_t digits[64] = {0};

    digits[0] = 3;  /* leading digit fixed to 3 */

    /* Fill the remaining free digits of the first half from most
       significant to least significant using base‑9 representation
       of 'remaining'. */
    for (int i = halfLen - 1; i >= 1; --i) {
        digits[i] = (uint8_t)(remaining % 9);
        remaining /= 9;
    }

    /* Mirror to form the full palindrome. */
    for (int i = 0; i < L / 2; ++i) {
        digits[L - 1 - i] = digits[i];
    }

    /* Convert the base‑9 digits to a decimal value. */
    seq_t value = 0;
    for (int i = 0; i < L; ++i) {
        value = value * 9 + digits[i];
    }

    return value;
}
