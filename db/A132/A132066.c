/*
 * A132066  Irregular array: the sum of the first m terms of row n is the m-th
 *          positive divisor of n.
 * https://oeis.org/A132066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A132066.
   Each integer n ≥ 1 lies in a unique row r.
   Row r contains d(r) terms, where d(r) is the number of
   positive divisors of r.  If d₁ < d₂ < … < d_{d(r)} are the
   divisors of r, the corresponding row terms are
   a = d₁ , d₂–d₁ , d₃–d₂ , … , d_{d(r)}–d_{d(r)–1}.
   The sequence a(n) is obtained by reading the rows
   a(1) a(2) … in order.  */

/* Required types are supplied by the build configuration. */
#include <stdint.h>
#include <stdlib.h>

/* Comparison function for qsort on idx_t values. */
static int cmp_idx_t(const void *a, const void *b)
{
    const idx_t *va = (const idx_t *)a;
    const idx_t *vb = (const idx_t *)b;
    return (*va > *vb) - (*va < *vb);
}

/* Count the number of positive divisors of n (d(n)). */
static idx_t divisor_count(idx_t n)
{
    idx_t cnt = 1;
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            cnt *= (e + 1);
        }
    }
    if (temp > 1) cnt *= 2;
    return cnt;
}

/* Return all positive divisors of n in ascending order.
   The caller must free the returned array. */
static void get_divisors(idx_t n, idx_t **out, size_t *len)
{
    size_t cap = 16;
    idx_t *arr = (idx_t *)malloc(cap * sizeof(idx_t));
    size_t l = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if (l == cap) {
                cap <<= 1;
                arr = (idx_t *)realloc(arr, cap * sizeof(idx_t));
            }
            arr[l++] = i;
            idx_t j = n / i;
            if (j != i) {
                if (l == cap) {
                    cap <<= 1;
                    arr = (idx_t *)realloc(arr, cap * sizeof(idx_t));
                }
                arr[l++] = j;
            }
        }
    }

    qsort(arr, l, sizeof(idx_t), cmp_idx_t);
    *out = arr;
    *len = l;
}

/* Main function computing the n-th term of sequence A132066. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Locate the row containing the n-th element. */
    idx_t row = 1;
    idx_t start = 1;              /* linear index of the first element of row 'row' */
    while (1) {
        idx_t d = divisor_count(row);
        if (n < start + d) break;
        start += d;
        ++row;
    }

    idx_t m = n - start;          /* zero‑based position within the row */

    /* Compute sorted divisors of the row. */
    idx_t *divs = NULL;
    size_t div_len = 0;
    get_divisors(row, &divs, &div_len);

    seq_t result;
    if (m == 0) {
        result = (seq_t)divs[0];                  /* first term is the first divisor (1) */
    } else {
        result = (seq_t)(divs[m] - divs[m - 1]);  /* difference to previous divisor */
    }

    free(divs);
    return result;
}
