/*
 * A167765  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*(a(n-1)+…+a(n-14)) – 496*a(n-15) for n >= 16 a(0)=1,
 *            a(1)=33, a(2)=1056, a(3)=33792, a(4)=1081344, a(5)=34603008,
 *            a(6)=1107296256, a(7)=35433480192, a(8)=1133871366144,
 *            a(9)=36283883716608, a(10)=1161084278931456,
 *            a(11)=37154696925806592, a(12)=1188950301625810944,
 *            a(13)=380464096520259...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_seq_array, seq_from_str, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>              /* for size_t */
#include <stdbool.h>

#include "../utils/alloc_seq_array.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_from_str.h"

static seq_t *seq = NULL;        /* array of computed terms */
static size_t seq_len = 0;       /* number of terms stored (seq[0..seq_len-1]) */
static size_t seq_cap = 0;       /* allocated capacity */

/* initialize base values up to n = 15 */
static void init_base(void)
{
    const char *base_vals[] = {
        "1",
        "33",
        "1056",
        "33792",
        "1081344",
        "34603008",
        "1107296256",
        "35433480192",
        "1133871366144",
        "36283883716608",
        "1161084278931456",
        "37154696925806592",
        "1188950301625810944",
        "38046409652025950208",
        "1217485108864830406656",
        "38959523483674573012464"
    };
    const size_t count = sizeof(base_vals) / sizeof(base_vals[0]);   /* 16 */

    /* allocate space for at least 16 elements */
    if (seq_cap < count) {
        seq_t *tmp = seq_realloc(NULL, 0, count);
        seq = tmp;
        seq_cap = count;
    }
    for (size_t i = 0; i < count; ++i) {
        seq[i] = seq_from_str(base_vals[i]);
    }
    seq_len = count;
}

/* ensure that seq has capacity for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 8;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = seq_realloc(seq, seq_cap, new_cap);
    seq = tmp;
    seq_cap = new_cap;
}

/* Extend the sequence up to index n (inclusive), computing new terms using
   the recurrence a(n)=31*(a(n-1)+…+a(n-14))-496*a(n-15)   */
static void compute_up_to(idx_t n)
{
    if (seq == NULL) init_base();
    if ((size_t)n < seq_len) return;            /* already computed */
    ensure_capacity(n);
    for (idx_t k = seq_len; k <= n; ++k) {
        seq_t sum = 0;
        for (int i = 1; i <= 14; ++i) {
            sum += seq[k - i] * 31;             /* 31 * a(k-i) */
        }
        sum -= seq[k - 15] * 496;              /* -496 * a(k-15) */
        seq[k] = sum;
    }
    seq_len = (size_t)n + 1;
}

/* Public function */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return seq[n];
}
