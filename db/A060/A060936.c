/*
 * A060936  Multiples of 3 with digits grouped in pairs and leading zeros
 *          omitted.
 * https://oeis.org/A060936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   Multiples of 3 with digits grouped in pairs and leading zeros
 *            omitted a(n) is obtained by concatenating all positive multiples
 *            of 3, grouping their decimal digits into consecutive pairs
 *            (starting from the first digit), then dropping a leading zero in
 *            each pair if present
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper arrays to store cumulative number of digits of successive multiples of 3 */
static uint64_t *cumulative = NULL;   /* cumulative[k] = total digits up to k-th multiple (1‑based) */
static size_t cumulative_size = 0;    /* number of entries stored */
static size_t cumulative_cap   = 0;    /* current capacity */

/* Ensure cumulative array contains an entry > pos (0‑based digit index) */
static void ensure_up_to(uint64_t pos)
{
    if (cumulative == NULL) {
        cumulative_cap = 64;
        cumulative = (uint64_t*)malloc(cumulative_cap * sizeof(uint64_t));
        cumulative_size = 0;
    }
    while (cumulative_size == 0 || cumulative[cumulative_size - 1] <= pos) {
        /* generate next multiple of 3 */
        uint64_t m = 3ULL * (cumulative_size + 1);   /* (1‑based index) */
        /* count digits of m */
        uint64_t tmp = m;
        uint64_t len = 0;
        do {
            tmp /= 10;
            ++len;
        } while (tmp);
        /* cumulative sum */
        uint64_t new_cum = (cumulative_size == 0 ? 0 : cumulative[cumulative_size - 1]) + len;
        if (cumulative_size == cumulative_cap) {
            cumulative_cap *= 2;
            cumulative = (uint64_t*)realloc(cumulative, cumulative_cap * sizeof(uint64_t));
        }
        cumulative[cumulative_size++] = new_cum;
    }
}

/* Return the decimal digit (0‑9) at 0‑based global position pos in the concatenated
   string of multiples of 3. */
static int get_digit_at(uint64_t pos)
{
    /* Binary search for first cumulative > pos */
    size_t lo = 0, hi = cumulative_size - 1;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (cumulative[mid] <= pos)
            lo = mid + 1;
        else
            hi = mid;
    }
    size_t k = lo; /* 0‑based index of multiple (k=0 => 3, k=1 => 6, ...) */
    uint64_t m = 3ULL * (k + 1);
    uint64_t prev_cum = (k == 0) ? 0 : cumulative[k - 1];
    uint64_t offset = pos - prev_cum;  /* 0‑based within digits of m (most significant digit) */

    /* Extract most significant digit at given offset */
    uint64_t tmp = m;
    uint64_t len = 0;
    uint64_t digits[20];
    do {
        digits[len++] = tmp % 10;
        tmp /= 10;
    } while (tmp);
    /* digits[0] is least significant, digits[len-1] is most significant */
    return (int)digits[len - 1 - offset];
}

seq_t a(idx_t n)
{
    /* n >= 1, compute pair starting at position 2*(n-1) and 2*(n-1)+1 */
    uint64_t p = 2ULL * (n - 1);
    /* Need to ensure we have enough digits for p+1 */
    ensure_up_to(p + 1);
    int d1 = get_digit_at(p);
    int d2 = get_digit_at(p + 1);
    seq_t result;
    if (d1 == 0) {
        result = (seq_t)d2;       /* leading zero omitted */
    } else {
        result = (seq_t)(d1 * 10 + d2);
    }
    return result;
}
