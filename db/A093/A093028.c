/*
 * A093028  Indices of terms in A093019 with value 8.
 * https://oeis.org/A093028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the n-th positive integer whose Luhn check digit (as
 *            used in credit‑card numbers) equals 8. The sequence is not given
 *            by a closed formula, so we compute it by scanning successive
 *            integers
 * Range:     u8 n <= 25, u16 n <= 6554, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9774 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the Luhn check digit for a single decimal integer `num`.
   Returns a value 0–9.  The algorithm is the same for all
   supported integer types. */
static int luhn_check_digit(seq_t num)
{
    seq_t tmp = num;
    int sum  = 0;
    int pos  = 1;          /* 1 for the rightmost digit of num */
    while (tmp > 0) {
        int d = (int)(tmp % 10);          /* current digit */
        if (pos % 2 == 1) {               /* double every other digit */
            d *= 2;
            if (d > 9) d -= 9;
        }
        sum += d;
        pos++;
        tmp /= 10;
    }
    return (10 - (sum % 10)) % 10;
}

/* Return the n-th element of A093028 (indices of A093019 with value 8).
   The function works for any supported combination of seq_t and idx_t.
   It scans successive numbers starting at 1 and counts those whose
   Luhn check digit is 8 until the desired position is reached. */
seq_t a(idx_t n)
{
    idx_t   count = 0;
    seq_t   num   = 1;

    for (;; ++num) {
        if (luhn_check_digit(num) == 8) {
            ++count;
            if (count == n)
                return num;
        }
    }
}
