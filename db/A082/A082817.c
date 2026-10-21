/*
 * A082817  Triangle (with distinct entries) read by rows in which row n
 *          contains n numbers whose product == -1 (mod n).
 * https://oeis.org/A082817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Construct sequence A082817. For each n, row n contains n
 *            distinct integers unused previously, all coprime to n, with
 *            product ≡ -1 (mod n). The first n‑1 entries of a row are the
 *            smallest unused coprime numbers; the last entry is chosen to
 *            satisfy the product modulo n. The sequence is read row by row
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   modinv_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/modinv_u64.h"

/* gcd for uint64_t */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* -------------------------------- bitset for used numbers -------------------- */
static uint8_t *used_bits = NULL;   /* each bit = used */
static size_t used_bits_len = 0;    /* in bytes */

/* ensure bitset covers numbers up to v */
static void ensure_used_capacity(uint64_t v)
{
    size_t bits_needed = v + 1;
    size_t bytes_needed = (bits_needed + 7) >> 3;
    if (bytes_needed > used_bits_len) {
        size_t new_len = bytes_needed * 2;
        uint8_t *new_bits = (uint8_t *)realloc(used_bits, new_len);
        if (!new_bits)
            exit(EXIT_FAILURE);
        /* zero the newly allocated part */
        memset(new_bits + used_bits_len, 0, new_len - used_bits_len);
        used_bits = new_bits;
        used_bits_len = new_len;
    }
}

static inline bool is_used_uint64(uint64_t v)
{
    ensure_used_capacity(v);
    size_t idx = v >> 3;
    unsigned char mask = 1 << (v & 7);
    return (used_bits[idx] & mask) != 0;
}

static inline void set_used_uint64(uint64_t v)
{
    ensure_used_capacity(v);
    size_t idx = v >> 3;
    unsigned char mask = 1 << (v & 7);
    used_bits[idx] |= mask;
}

/* -------------------------------- dynamic arrays for values and rows -------------------- */
static seq_t *values = NULL;
static size_t values_len = 0, values_cap = 0;

static size_t *row_start = NULL;
static size_t *row_size = NULL;   /* not strictly needed */
static size_t rows_cap = 0;

static size_t max_row_computed = 0;

/* ensure row arrays have capacity for at least `need` rows */
static void ensure_rows_capacity(size_t need)
{
    if (need <= rows_cap)
        return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 1;
    while (new_cap < need)
        new_cap <<= 1;
    size_t *tmp = (size_t *)realloc(row_start, new_cap * sizeof(size_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    row_start = tmp;
    tmp = (size_t *)realloc(row_size, new_cap * sizeof(size_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    row_size = tmp;
    rows_cap = new_cap;
}

/* ensure values array can hold at least `need` elements */
static void ensure_values_capacity(size_t need)
{
    if (need <= values_cap)
        return;
    size_t new_cap = values_cap ? values_cap * 2 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    values = tmp;
    values_cap = new_cap;
}

/* generate rows up to and including row `r` (1‑based) */
static void ensure_row(size_t r)
{
    if (r <= max_row_computed)
        return;

    if (rows_cap < r)
        ensure_rows_capacity(r);

    for (size_t cur = max_row_computed + 1; cur <= r; ++cur) {
        size_t before = values_len;

        /* special case for row 1 */
        if (cur == 1) {
            set_used_uint64(1);
            ensure_values_capacity(values_len + 1);
            values[values_len++] = (seq_t)1;
            row_start[cur - 1] = before;
            row_size[cur - 1] = cur;
            continue;
        }

        size_t found = 0;
        uint64_t cand = 1;
        __uint128_t prod_mod = 1 % cur;   /* product mod cur */

        while (found < cur - 1) {
            /* find next unused coprime candidate */
            while (true) {
                ensure_used_capacity(cand);
                if (is_used_uint64(cand))
                    cand++;
                else if (gcd_u64(cand, cur) != 1)
                    cand++;
                else
                    break;
            }
            /* use candidate */
            uint64_t val64 = cand;
            seq_t val = (seq_t)val64;
            ensure_values_capacity(values_len + 1);
            values[values_len++] = val;
            set_used_uint64(cand);
            prod_mod = (prod_mod * (cand % cur)) % cur;
            found++;
            cand++;
        }

        /* last entry to satisfy product ≡ -1 (mod cur) */
        uint64_t inv_prod = modinv_u64((uint64_t)prod_mod, cur);
        uint64_t last_residue = ((cur - 1) * inv_prod) % cur;
        uint64_t last_val_candidate = last_residue;
        /* search for unused, still coprime */
        while (true) {
            ensure_used_capacity(last_val_candidate);
            if (!is_used_uint64(last_val_candidate) &&
                gcd_u64(last_val_candidate, cur) == 1)
                break;
            last_val_candidate += cur;
        }
        seq_t last_val = (seq_t)last_val_candidate;
        ensure_values_capacity(values_len + 1);
        values[values_len++] = last_val;
        set_used_uint64(last_val_candidate);

        /* record row meta data */
        row_start[cur - 1] = before;
        row_size[cur - 1] = cur;
    }

    max_row_computed = r;
}

/* find row r such that r*(r+1)/2 >= idx (1‑based) */
static size_t find_row(uint64_t idx)
{
    double rd = (sqrt(8.0 * idx + 1.0) - 1.0) / 2.0;
    return (size_t)ceil(rd);
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)           /* offset starts at 1 */
        return (seq_t)0;

    uint64_t idx = (uint64_t)n;
    size_t r = find_row(idx);
    size_t pos = idx - ((uint64_t)r * (r - 1) / 2) - 1; /* 0‑based in row */

    ensure_row(r);

    size_t start = row_start[r - 1];
    return values[start + pos];
}
