/*
 * A079826  Consider the triangle shown below; sequence contains the
 *          concatenation of numbers read at a 45-degree angle upwards with
 *          horizontal beginning with the first term of a row.
 * https://oeis.org/A079826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079826:
 * a(n) = decimal concatenation of the integers on the nth diagonal of the triangle
 * whose rows contain consecutive integers: odd‑numbered rows are written left‑to‑right,
 * even‑numbered rows are written right‑to‑left.
 * The nth diagonal contains ceil(n/2) numbers.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t pow10_tbl[11];

/* Helper to ensure pow10_tbl is initialized once */
static void init_pow10(void)
{
    static bool ready = false;
    if (!ready) {
        pow10_tbl[0] = (seq_t)1;
        for (int i = 1; i < 11; ++i)
            pow10_tbl[i] = pow10_tbl[i-1] * (seq_t)10;
        ready = true;
    }
}

seq_t a(idx_t n)
{
    init_pow10();

    seq_t result = (seq_t)0;

    /* Length of the nth diagonal = ceil(n/2) = (n+1)/2 */
    idx_t diag_len = (n + 1) / 2;

    /* The smallest row index that appears in this diagonal */
    idx_t start_row = n - diag_len + 1;

    for (idx_t r = n; r >= start_row; --r) {
        idx_t col = n - r + 1;                      /* column within row r */

        /* start = (r-1)*r/2 + 1 */
        seq_t start = ((seq_t)(r-1) * (seq_t)r) / 2 + (seq_t)1;

        seq_t val;
        if (r & 1)                                  /* odd row: ascending */
            val = start + (seq_t)(col - 1);
        else                                         /* even row: descending */
            val = start + (seq_t)(r - col);

        /* compute number of decimal digits of val (val <= 50,000,000 for n<=10000) */
        unsigned long long uv = (unsigned long long)val; /* guaranteed small */
        int digits = 0;
        do {
            ++digits;
            uv /= 10;
        } while (uv);

        /* concatenate: result = result * 10^digits + val */
        result = result * pow10_tbl[digits] + val;
    }

    return result;
}
