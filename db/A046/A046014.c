/*
 * A046014  Discriminants of imaginary quadratic fields with class number 17
 *          (negated).
 * https://oeis.org/A046014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating fundamental discriminants with class
   number 17 and collecting their absolute values in increasing
   order. No closed form is known. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define NUM_VALUES 45              /* known number of terms */

static idx_t isqrt_idx(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid > 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return high;
}

/* Test whether an unsigned 64‑bit integer is square‑free. */
static bool is_squarefree_uint64(uint64_t n) {
    if (n <= 1) return true;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return false;
            while (n % p == 0) n /= p;
        }
    }
    return true;
}

/* Test if a negative integer D is a fundamental discriminant. */
static bool is_fundamental_discriminant(int D) {
    /* D is negative. */
    if ((D & 3) == 0) {              /* D ≡ 0 (mod 4) */
        int D4 = D / 4;              /* truncates toward zero */
        if ((D4 & 3) == 2 || (D4 & 3) == 3) {
            return is_squarefree_uint64((uint64_t)(- (int64_t)D4));
        }
        return false;
    } else if ((D & 3) == 1) {       /* D ≡ 1 (mod 4) */
        return is_squarefree_uint64((uint64_t)(- (int64_t)D));
    }
    return false;
}

/* Count reduced binary quadratic forms with negative discriminant D.
   The count equals the class number h(D).  The routine stops early if
   more than 17 forms are found. */
static seq_t class_number_of_D(int D) {
    if (D >= 0) return 0;           /* only negative discriminants */
    uint64_t absD = (uint64_t)(- (int64_t)D);
    int count = 0;

    uint64_t a_limit = isqrt_idx(absD / 3);
    for (uint64_t a = 1; a <= a_limit; ++a) {
        int64_t b_start = -(int64_t)a;
        int64_t b_end   =  (int64_t)a;
        for (int64_t b = b_start; b <= b_end; ++b) {
            if (((b + D) & 1) != 0) continue;     /* parity condition */
            int64_t B2 = b * b;
            int64_t num = B2 - (int64_t)D;        /* B^2 + |D| */
            int64_t denom = 4 * (int64_t)a;
            if (num % denom != 0) continue;
            int64_t c = num / denom;
            if (c < a) continue;                 /* reduced: a ≤ c */

            /* Reduced requires: |b| ≤ a ≤ c and if a = |b| or a = c,
               then b ≥ 0. */
            int64_t absb = b < 0 ? -b : b;
            if ((a == c || a == absb) && b < 0) continue;

            ++count;
            if (count > 17) return (seq_t)count;  /* early exit */
        }
    }
    return (seq_t)count;
}

/* Storage for the 45 known discriminants. */
static seq_t known_values[NUM_VALUES];
static size_t known_count = 0;
static bool generated = false;

/* Populate the known_values array on first use. */
static void generate_sequence(void) {
    if (generated) return;
    for (int64_t D = -3; known_count < NUM_VALUES; --D) {
        if (!is_fundamental_discriminant((int)D)) continue;
        seq_t h = class_number_of_D((int)D);
        if (h == 17) {
            known_values[known_count++] = (seq_t)(- (int64_t)D);
        }
    }
    generated = true;
}

/* Public function required by the build system. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined below offset */
    generate_sequence();
    if (n <= known_count) {
        return known_values[(size_t)n - 1];
    }
    return 0;                      /* undefined for n > 45 */
}
