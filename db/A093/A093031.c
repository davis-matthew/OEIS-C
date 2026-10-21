/*
 * A093031  Create a histogram of the digits used so far in the sequence. Add
 *          to the last element of the sequence the least used digit on the
 *          histogram. If two or more digits have the same score in the
 *          histogram, add the smallest. 0 is to be interpreted as 10 and so
 *          comes after 9. So if 0 is the digit to add...
 * https://oeis.org/A093031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 7445, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Created from OEIS A093031: 
   a(1) = 0. For n > 1,
   let d be the digit (0‑9) with the smallest count in the decimal
   representations of a(1)…a(n‑1).  If 0 has the same smallest count,
   it is considered larger than 9, so the smallest among the tied digits
   is chosen.  If 0 is the least frequent digit, add 10 instead of 0.
   Then a(n) = a(n‑1) + d (with d = 10 when 0 is chosen).
*/
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *seq = NULL;        /* array of terms */
static size_t seq_cap = 0;       /* allocated capacity */
static size_t seq_len = 0;       /* current number of terms */
static size_t hist[10] = {0};    /* histogram of digits (0‑9) */

/* ensure that the array can hold at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap) return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq = tmp;
    seq_cap = new_cap;
}

/* update the histogram with the decimal digits of x */
static void add_digits_to_hist(seq_t x)
{
    if (x == 0) {
        hist[0]++; 
        return;
    }
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        hist[d]++;
        x /= 10;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, return 0 for protection */

    size_t target = (size_t)n;            /* n is uint64_t, fits in size_t */

    /* initialize first term if not already done */
    if (seq_len == 0) {
        ensure_capacity(1);
        seq[0] = (seq_t)0;
        seq_len = 1;
        for (int i = 0; i < 10; ++i) hist[i] = 0;
        hist[0] = 1;                      /* digit '0' from the first term */
    }

    /* compute terms up to 'n' if necessary */
    while (seq_len < target) {
        /* find minimal frequency among 0‑9 */
        size_t minfreq = hist[0];
        for (int d = 1; d < 10; ++d) {
            if (hist[d] < minfreq) minfreq = hist[d];
        }

        /* find the digit with that frequency that is smallest when 0 is
           considered as 10 (i.e., after all other digits) */
        int chosen_digit = -1;     /* real digit 0‑9 that will be added */
        int chosen_order = 11;     /* order value (0→10, 1‑9→its value) */

        for (int d = 0; d < 10; ++d) {
            if (hist[d] == minfreq) {
                int order = (d == 0) ? 10 : d;
                if (order < chosen_order) {
                    chosen_order = order;
                    chosen_digit = d;
                }
            }
        }

        int add_value = (chosen_digit == 0) ? 10 : chosen_digit;
        seq_t last = seq[seq_len - 1];
        seq_t next = last + (seq_t)add_value;

        /* append next term */
        ensure_capacity(seq_len + 1);
        seq[seq_len] = next;
        ++seq_len;

        /* update histogram with digits of the new term */
        add_digits_to_hist(next);
    }

    return seq[target - 1];
}
