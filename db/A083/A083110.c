/*
 * A083110  Triangle read by rows in which the n-th row contains
 *          lexicographically earliest increasing sequence of n positive
 *          integers whose product == -1 (mod n).
 * https://oeis.org/A083110
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A083110
    Formula: a(n) is the element at position n in the triangle
    whose k‑th row consists of the lexicographically earliest
    increasing sequence of k positive integers whose product ≡ –1 (mod k).
    The triangle is flattened by rows and the sequence is indexed
    with offset 2 (so a(2) is the first element, the 1 of the first row).  */

#include <stdint.h>
#include <stdbool.h>

/* ---------- utility functions ---------- */

/* Integer gcd for 64 bit values */
static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Integer square root using binary search on idx_t (uint64_t) */
static inline idx_t isqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        /* use 128 bit to avoid overflow */
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Brute‑force modular inverse, valid because k ≤ 10000 in practice */
static inline uint64_t mod_inv(uint64_t a, uint64_t mod)
{
    for (uint64_t x = 1; x < mod; ++x)
        if ((a * x) % mod == 1)
            return x;
    return 0; /* should never happen for a unit modulo mod */
}

/* ---------- sequence computation ---------- */

seq_t a(idx_t n)
{
    /* n ≥ 2 is guaranteed by caller; handle n = 2 → first element of row 1 */
    idx_t m = n - 1;                   /* zero‑based position in flattened triangle */

    /* Find the row length k such that sum_{t=1}^{k-1} t  < m ≤ sum_{t=1}^{k} t   */
    idx_t k = (isqrt(8 * m + 1) - 1) >> 1;
    while (k * (k + 1) / 2 < m) ++k;
    while ((k - 1) * k / 2 >= m) --k;

    /* Position inside the k‑th row (1‑indexed) */
    idx_t pos = m - (k - 1) * k / 2;    /* 1 ≤ pos ≤ k */

    /* Special case: row of length 1 (k == 1), the sequence is simply [1] */
    if (k == 1) {
        return (seq_t)1;
    }

    /* If the required element is among the first k‑1 coprime numbers,
       generate just enough to reach that position. */
    if (pos <= k - 1) {
        idx_t count = 0;
        idx_t candidate = 1;
        seq_t result = 0;

        while (count < pos) {
            if (gcd64(candidate, k) == 1) {
                ++count;
                if (count == pos)
                    result = (seq_t)candidate;
            }
            ++candidate;
        }
        return result;
    }

    /* The requested element is the last one of the row. */
    /* First collect the first k‑1 coprime numbers. */
    idx_t count = 0;
    idx_t candidate = 1;
    uint64_t product_mod = 1;          /* product modulo k */
    uint64_t last = 0;                 /* last value chosen */

    while (count < k - 1) {
        if (gcd64(candidate, k) == 1) {
            product_mod = (product_mod * (candidate % k)) % k;
            last = candidate;
            ++count;
        }
        ++candidate;
    }

    /* Compute the residue that the final number must have modulo k */
    uint64_t invP = mod_inv(product_mod % k, k);          /* inverse of product modulo k */
    uint64_t target_residue = ((k - 1) * invP) % k;       /* (−1)·invP  (mod k) */

    /* Find the smallest integer > last with this residue modulo k */
    int64_t diff = (int64_t)(last + 1) - (int64_t)target_residue;
    uint64_t t = 0;
    if (diff > 0)
        t = ((diff + (int64_t)k - 1) / (int64_t)k);       /* ceil division */

    uint64_t last_value = target_residue + t * k;

    return (seq_t)last_value;
}
