/*
 * A021069  Decimal expansion of 1/65.
 * https://oeis.org/A021069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit of the decimal expansion of 1/65 with a(0) = 0 (the
 *            integer part) and a(n) for n≥1 taken from the fractional part
 *            after skipping the leading 0 after the decimal point
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *frac_digits = NULL;  /* digits of 1/65 after decimal starting with first 0 */
static idx_t frac_len   = 0;       /* number of digits stored */
static idx_t frac_cap   = 0;       /* allocated capacity */
static unsigned long long rem = 0; /* current remainder of 1/65 */

/* Ensure the fractional digits array can hold 'needed' elements. */
static void ensure_frac_capacity(idx_t needed) {
    if (needed <= frac_cap) return;
    idx_t new_cap = frac_cap ? frac_cap << 1 : 1024;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(frac_digits, new_cap * sizeof(seq_t));
    frac_digits = new_ptr;
    frac_cap = new_cap;
}

/* Compute digit of 1/65 at 0‑based index n. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* integer part */

    /* Initialize static state on first use. */
    if (frac_digits == NULL) {
        frac_cap = 1024;
        frac_digits = (seq_t *)malloc(frac_cap * sizeof(seq_t));
        frac_len = 0;
        rem = 1ULL % 65ULL;  /* start remainder with numerator 1         */
    }

    /* The (n‑th) term corresponds to the nth fractional digit
       after the leading zero, that is, the (n+1)-th fractional digit.
       We need digits[0] .. digits[n] (total n+1 digits).          */
    idx_t needed = n + 1;
    if (needed > frac_len) {
        ensure_frac_capacity(needed);
        while (frac_len < needed) {
            /* compute next fractional digit */
            unsigned long long r10 = rem * 10ULL;
            seq_t digit = (seq_t)(r10 / 65ULL);
            rem = r10 % 65ULL;
            frac_digits[frac_len++] = digit;
        }
    }

    return frac_digits[n];
}
