/*
 * A191432  Dispersion of ([n*x+1/x]), where x=sqrt(2) and [ ]=floor, by
 *          antidiagonals.
 * https://oeis.org/A191432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = element at the n‑th position of the dispersal array
 *            generated from s(k) = floor((2k+1)/sqrt(2)) by iterating s on
 *            the complement sequence t (with t(1)=1) along antidiagonals
 * Range:     u8 n <= 80, u16 n <= 408, u32 n <= 1832, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation works for seq_t of 64 or 128 bits.
   It avoids overflow in the computation of s(x) by using 128‑bit
   arithmetic and an integer square‑root routine. */

#include <stdint.h>
#include <stdbool.h>

/* integer square root of a 128‑bit unsigned integer */
static inline unsigned long long isqrt128(unsigned __int128 v) {
    unsigned long long r = 0;
    for (int i = 63; i >= 0; --i) {
        unsigned long long bit = 1ULL << i;
        unsigned long long cand = r | bit;
        unsigned __int128 cand2 = (unsigned __int128)cand * cand;
        if (cand2 <= v) r = cand;
    }
    return r;
}

/* compute s(n) = floor((2n+1) / sqrt(2)) */
static inline seq_t s_of(seq_t n) {
    /* use 128‑bit intermediate to avoid overflow */
    unsigned __int128 a = (unsigned __int128)n * 2 + 1;          /* 2n+1 */
    unsigned __int128 prod = a * a;                             /* (2n+1)^2 */
    unsigned __int128 div = prod >> 1;                          /* floor(prod/2) */
    unsigned long long root = isqrt128(div);                    /* floor(sqrt(div)) */
    return (seq_t)root;
}

/* r‑th element of the complement sequence t (with t(1)=1) */
static inline seq_t t_of(idx_t r) {
    idx_t count = 0;
    seq_t i = 1;          /* integer being tested for membership in s */
    seq_t k = 1;          /* index used to generate s(k) */
    seq_t next_s = s_of(k);

    while (count < r) {
        /* advance k until next_s ≥ i */
        while (next_s < i) {
            ++k;
            next_s = s_of(k);
        }
        /* i is not in s -> it is in the complement */
        if (next_s != i) {
            ++count;
            if (count == r) return i;
        }
        ++i;
    }
    /* unreachable */
    return i;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* convert to 0‑based index */
    idx_t m = n - 1;

    /* determine diagonal D: smallest D such that D*(D+1)/2 > m */
    /* D = floor((sqrt(8*m+1)-1)/2)+1   (for 0‑based index) */
    unsigned __int128 t_val = (unsigned __int128)8 * m + 1;
    /* sqrt(8*m+1) fits in 128 bits */
    unsigned long long t_sqrt = isqrt128(t_val);
    idx_t D = (t_sqrt - 1) / 2 + 1;            /* diagonal number (≥1) */

    /* number of elements before this diagonal */
    idx_t elems_before = (D - 1) * D / 2;

    /* position inside diagonal (0‑based) */
    idx_t pos = m - elems_before;

    /* row and column within the diagonal */
    idx_t row = pos + 1;            /* 1‑based */
    idx_t col = D - pos;            /* 1‑based */

    /* starting value: t(row) */
    seq_t x = t_of(row);

    /* apply s col‑1 times */
    for (idx_t i = 1; i < col; ++i)
        x = s_of(x);

    return x;
}
