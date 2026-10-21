/*
 * A083106  Triangle read by rows in which the n-th row contains n smallest
 *          numbers whose product == 1 mod n.
 * https://oeis.org/A083106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value at position n in the triangle where the r‑th row
 *            consists of the r smallest positive integers whose product is 1
 *            modulo r. The algorithm generates the r first coprime numbers,
 *            adjusts the last element so that the product becomes 1, sorts
 *            the row, and returns the element at the reques...
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd64.h"

/* greatest common divisor for 64‑bit unsigned integers */

/* extended Euclidean algorithm (return g, store x,y s.t. ax+by=g) */
static int64_t egcd64(int64_t a, int64_t b, int64_t *x, int64_t *y)
{
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int64_t x1, y1;
    int64_t g = egcd64(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

/* modular inverse of a modulo m (m > 1, a coprime to m) */
static uint64_t mod_inv(uint64_t a, uint64_t m)
{
    int64_t x, y;
    egcd64((int64_t)a, (int64_t)m, &x, &y);
    if (x < 0)
        x += (int64_t)m;
    return (uint64_t)x;
}

/* comparison function for qsort on uint64_t */
static int cmp_uint64(const void *p, const void *q)
{
    const uint64_t *a = (const uint64_t *)p;
    const uint64_t *b = (const uint64_t *)q;
    return (*a < *b) ? -1 : (*a > *b) ? 1 : 0;
}

/* determine the row number r and the position pos (1‑based) of index n */
static void row_and_pos(idx_t n, idx_t *r, idx_t *pos)
{
    idx_t cur = 1;
    idx_t cum = 1;              /* sum 1..cur                     */
    while (cum < n) {
        cur++;
        cum += cur;
    }
    *r = cur;
    *pos = n - (cum - cur);    /* cum - cur = sum 1..cur-1        */
}

/* main sequence function */
seq_t a(idx_t n)
{
    idx_t row, pos;
    row_and_pos(n, &row, &pos);

    /* trivial case */
    if (row == 1)
        return (seq_t)1;

    /* allocate array for the row (use uint64_t internally) */
    uint64_t *row64 = (uint64_t *)malloc((size_t)row * sizeof(uint64_t));
    if (row64 == NULL)
        return (seq_t)0;  /* out of memory; caller can handle */

    /* collect the first "row" positive integers coprime to "row" */
    idx_t cnt = 0;
    for (uint64_t i = 1; cnt < row; i++) {
        if (gcd64(i, (uint64_t)row) == 1) {
            row64[cnt++] = i;
        }
    }

    /* product modulo row */
    uint64_t prod_mod = 1;
    for (idx_t k = 0; k < row; k++) {
        uint64_t v = row64[k] % (uint64_t)row;
        prod_mod = (uint64_t)(((__int128)prod_mod * v) % row);
    }

    /* if product is not 1 mod row, adjust the last element */
    if (prod_mod % row != 1) {
        /* product of the first row-1 elements */
        uint64_t prod_no_last = 1;
        for (idx_t k = 0; k < row - 1; k++) {
            uint64_t v = row64[k] % (uint64_t)row;
            prod_no_last = (uint64_t)(((__int128)prod_no_last * v) % row);
        }
        uint64_t inv = mod_inv(prod_no_last % row, (uint64_t)row);
        uint64_t target = inv % row;
        if (target == 0)
            target = row;               /* convert 0 residue to r itself   */

        /* find the smallest positive number ≡ target (mod row) not used yet */
        uint64_t candidate = target;
        bool used;
        do {
            used = false;
            for (idx_t k = 0; k < row - 1; k++) {
                if (row64[k] == candidate) {
                    used = true;
                    break;
                }
            }
            if (!used)
                break;
            candidate += row;
        } while (true);

        row64[row - 1] = candidate;     /* replace last element            */
    }

    /* sort the row to ensure ascending order */
    qsort(row64, (size_t)row, sizeof(uint64_t), cmp_uint64);

    seq_t result = (seq_t)row64[pos - 1];
    free(row64);
    return result;
}
