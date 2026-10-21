/*
 * A076450  Natural sculptures.
 * https://oeis.org/A076450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer whose sculpture vector is
 *            unique * across all smaller integers. The function below
 *            computes the sequence by * brute‑force trial factorisation and
 *            memoising already seen sculptures * (in both C11 and C++17
 *            builds)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------------ */
/* Sculpture representation:                                           */
/* ------------------------------------------------------------------ */
typedef struct {
    uint8_t len;          /* number of non‑zero c[k] values   */
    uint8_t counts[64];   /* c[1] .. c[64] (packed as 8‑bit) */
} Sculpture;

/* ------------------------------------------------------------------ */
/* Compute the sculpture vector for n (n >= 1).                         */
/* ------------------------------------------------------------------ */
static void sculpture_of(uint64_t n, Sculpture *res)
{
    uint8_t tmp[64] = {0};
    uint64_t m = n;

    /* trial division factorisation */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* find k such that p^k <= n < p^(k+1)  */
            int k = 1;
            uint64_t val = p;
            while (val <= n / p) {  /* avoid overflow */
                val *= p;
                ++k;
            }
            if (k > 63) k = 63;
            tmp[k-1] += (uint8_t)exp;
        }
    }
    if (m > 1) {                       /* remaining prime factor */
        uint64_t p = m;
        int k = 1;
        uint64_t val = p;
        while (val <= n / p) {
            val *= p;
            ++k;
        }
        if (k > 63) k = 63;
        tmp[k-1] += 1;
    }

    /* determine the actual length */
    int len = 0;
    for (int i = 63; i >= 0; --i) {
        if (tmp[i] != 0) { len = i + 1; break; }
    }
    res->len = (uint8_t)len;
    memcpy(res->counts, tmp, 64);
}

/* ------------------------------------------------------------------ */
/* Check whether a sculpture already exists in the stored list.        */
/* ------------------------------------------------------------------ */
static bool sculpture_exists(const Sculpture *s,
#if defined(__cplusplus)
                             const std::vector<Sculpture> *sculptures)
#else
                             const Sculpture *sculptures,
                             size_t sculptures_size)
#endif
{
#if defined(__cplusplus)
    for (const Sculpture &other : *sculptures) {
        if (other.len != s->len) continue;
        if (memcmp(other.counts, s->counts, s->len) == 0) return true;
    }
    return false;
#else
    for (size_t i = 0; i < sculptures_size; ++i) {
        if (sculptures[i].len != s->len) continue;
        if (memcmp(sculptures[i].counts, s->counts, s->len) == 0) return true;
    }
    return false;
#endif
}

/* ------------------------------------------------------------------ */
/* Main function requested.                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe */

#if defined(__cplusplus)
    /* C++ build: use std::vector for dynamic arrays.              */
    static std::vector<Sculpture> sculptures_vec;
    static std::vector<seq_t> values_vec;
    static uint64_t checked = 0;   /* last integer that has been processed */

    while (values_vec.size() < (size_t)n) {
        ++checked;                 /* next integer to consider */

        Sculpture s;
        sculpture_of(checked, &s);

        if (!sculpture_exists(&s, &sculptures_vec)) {
            sculptures_vec.push_back(s);
            values_vec.push_back((seq_t)checked);
        }
    }

    return values_vec[(size_t)(n - 1)];
#else
    /* C build: use malloc/realloc.                                 */
    static seq_t *values = NULL;
    static uint64_t values_size = 0, values_cap = 0;

    static Sculpture *sculptures = NULL;
    static size_t sculptures_size = 0, sculptures_cap = 0;

    static uint64_t checked = 0;   /* last integer that has been processed */

    while (values_size < n) {
        ++checked;                /* next integer to consider */

        Sculpture s;
        sculpture_of(checked, &s);

        if (!sculpture_exists(&s, sculptures, sculptures_size)) {
            /* store sculpture */
            if (sculptures_size >= sculptures_cap) {
                size_t new_cap = sculptures_cap ? sculptures_cap << 1 : 8;
                Sculpture *tmp = realloc(sculptures, new_cap * sizeof(Sculpture));
                sculptures = tmp;
                sculptures_cap = new_cap;
            }
            sculptures[sculptures_size++] = s;

            /* store value */
            if (values_size >= values_cap) {
                size_t new_cap = values_cap ? values_cap << 1 : 8;
                seq_t *tmp = realloc(values, new_cap * sizeof(seq_t));
                values = tmp;
                values_cap = new_cap;
            }
            values[values_size++] = (seq_t)checked;
        }
    }

    return values[(size_t)(n - 1)];
#endif
}
