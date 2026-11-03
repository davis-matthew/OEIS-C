/*
 * A139054  Array read by rows: row n lists the first 4 primes with n digits.
 * https://oeis.org/A139054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A139054: the first four primes with exactly d decimal digits
   for each digit length d, arranged row-wise.  The function generates the
   required primes on demand and caches them.  It works for any seq_t
   supported by the build, including arbitrary-precision integers. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Each entry stores the four primes for a given digit length. */
typedef struct {
    seq_t primes[4];
    bool  computed;
} row_t;

static row_t *rows   = NULL;   /* dynamic array indexed by digit length  - 1 */
static size_t rows_cap = 0;    /* allocated capacity (number of rows) */

/* Ensure that rows[0..max_d-1] exist.  Grow the table if necessary. */
static void ensure_rows(idx_t max_d)
{
    if (max_d + (idx_t)1 <= (idx_t)rows_cap)
        return;

    size_t new_cap = (size_t)(max_d + (idx_t)1);
    row_t *tmp = mem_resize(row_t, rows, new_cap);
    if (!tmp) /* allocation failed – leave state unchanged */
        return;

    /* Zero‑initialize any newly added rows */
    size_t old_bytes = rows_cap * sizeof(row_t);
    size_t new_bytes = new_cap * sizeof(row_t);
    memset((char *)tmp + old_bytes, 0, new_bytes - old_bytes);

    rows = tmp;
    rows_cap = new_cap;
}

/* Generate the first four primes with exactly 'digits' decimal digits
   and store them in rows[digits-1]. */
static void compute_primes_for_digit(idx_t digits)
{
    row_t *r = &rows[digits - 1];

    seq_t candidate = pow10(digits - 1);
    if (candidate <= (seq_t)2)
        candidate = (seq_t)2;

    idx_t found = 0;
    seq_t one   = (seq_t)1;
    while (found < 4) {
        if (is_prime_big(candidate)) {
            r->primes[found] = candidate;
            ++found;
        }
        candidate = candidate + one;
    }
    r->computed = true;
}

seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for n < 1 – return 0 */
        return (seq_t)0;

    /* Determine the digit length (row) and column position. */
    idx_t digit_len = ((n - 1) / 4) + 1;   /* row number (digit length)   */
    idx_t pos_in_row = (n - 1) % 4;        /* 0‑based position in the row */

    /* Ensure the rows array is large enough. */
    ensure_rows(digit_len);
    if (rows == NULL)          /* allocation failed */
        return (seq_t)0;

    row_t *row = &rows[digit_len - 1];
    if (!row->computed)
        compute_primes_for_digit(digit_len);

    return row->primes[pos_in_row];
}
