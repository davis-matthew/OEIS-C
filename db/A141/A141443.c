/*
 * A141443  Conjectured to be complete list of numbers not of the form 2*x^2 +
 *          y^2 + 12*triangular number.
 * https://oeis.org/A141443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers NOT representable as 2*x^2 + y^2 +
 *            12*triangular(z) (with x,y,z ≥ 0)
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_VAL 5000000ULL   /* upper bound for precomputation */

/* ------------------------------------------------------------ */
/*  Precomputed data for quick membership checks                */
/* ------------------------------------------------------------ */
static uint8_t *repR = NULL;        /* repR[v] == 1  ⇔ v = 2*x^2 + y^2  (≤ MAX_VAL) */
static uint64_t *offsets = NULL;    /* 12*triangular(z) = 6*z*(z+1)  ,  z ≤ zmax */
static int zmax = 0;
static bool initialized = false;

/* initialize tables once */
static void init_buffers(void) {
    if (initialized) return;
    repR = (uint8_t*)calloc((size_t)MAX_VAL + 1, 1);

    /* generate values of 2*x^2 + y^2 */
    for (uint64_t x = 0;; ++x) {
        uint64_t x2 = 2ULL * x * x;
        if (x2 > MAX_VAL) break;
        for (uint64_t y = 0;; ++y) {
            uint64_t val = x2 + y * y;
            if (val > MAX_VAL) break;
            repR[val] = 1;
        }
    }

    /* precompute offsets 6*z*(z+1) */
    uint64_t z = 0;
    while (6ULL * z * (z + 1) <= MAX_VAL) ++z;
    zmax = (int)(z - 1);
    offsets = (uint64_t*)malloc((zmax + 1) * sizeof(uint64_t));
    for (int i = 0; i <= zmax; ++i) {
        offsets[i] = 6ULL * (uint64_t)i * ((uint64_t)i + 1ULL);
    }

    initialized = true;
}

/* test whether a given number is representable */
static bool isRepresentable(uint64_t m) {
    for (int i = 0; i <= zmax; ++i) {
        uint64_t s = offsets[i];
        if (s > m) break;
        uint64_t r = m - s;
        if (repR[r]) return true;
    }
    return false;
}

/* ------------------------------------------------------------ */
/*  Memoized sequence storage                                  */
/* ------------------------------------------------------------ */
static seq_t *sequence = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static uint64_t next_candidate = 1;

/* grow the sequence array if needed and fill until length n */
static void ensure_sequence_size(idx_t n) {
    init_buffers();                     /* make sure tables exist */
    while (seq_len < (size_t)n) {
        uint64_t m = next_candidate;
        for (;;) {
            if (!isRepresentable(m)) {  /* m is a term of the sequence */
                if (seq_len == seq_cap) {                /* extend storage */
                    size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
                    seq_t *tmp = (seq_t*)realloc(sequence,
                                                 new_cap * sizeof(seq_t));
                    sequence = tmp;
                    seq_cap = new_cap;
                }
                sequence[seq_len++] = (seq_t)m;
                next_candidate = m + 1;
                break;
            }
            ++m;
        }
    }
}

/* ------------------------------------------------------------ */
/*  Public function as requested                               */
/* ------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* defensive, offset 1 is used in OEIS */
    ensure_sequence_size(n);
    return sequence[(size_t)(n - 1)];
}
