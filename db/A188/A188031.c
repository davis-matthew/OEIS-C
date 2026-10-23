/*
 * A188031  [nr]-[nr-kr]-[kr], where r=(1+sqrt(5))/2, k=11, [ ]=floor.
 * https://oeis.org/A188031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n·φ) – floor((n–11)·φ) – 17, where φ = (1+√5)/2 and
 *            the floor is taken on integers. The implementation computes
 *            floor(k·φ) for any integer k. It handles negative arguments
 *            needed for n < 11
 * Range:     u8 none, u16 n <= 57, u32 n <= 131, u64 n <= 131, u128 n <= 131,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* floor(k·φ) for integer k (k can be negative, but magnitude small for n < 11). */
static inline long long floor_k_phi_signed(long long k) {
    if (k >= 0) {
        /* k is non‑negative */
        seq_t kk = (seq_t)k;
        seq_t sq5 = (seq_t)5 * kk * kk;
        seq_t s = floor_sqrt(sq5);
        return (long long)((kk + s) / (seq_t)2);
    } else {
        /* k is negative: floor(k·φ) = –(floor(|k|·φ) + 1) */
        unsigned long long m = (unsigned long long)(-k);
        seq_t mm = (seq_t)m;
        seq_t sq5 = (seq_t)5 * mm * mm;
        seq_t s = floor_sqrt(sq5);
        seq_t fp = (mm + s) / (seq_t)2;          /* floor(|k|·φ) */
        return -(long long)(fp + (seq_t)1);
    }
}

/* floor(n·φ) for non‑negative n using floor(sqrt) */
static inline seq_t floor_n_phi_unsigned(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t sq5 = (seq_t)5 * nn * nn;
    seq_t s = floor_sqrt(sq5);
    return (nn + s) / (seq_t)2;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;                 /* values below offset are irrelevant */
    seq_t fn = floor_n_phi_unsigned(n);

    if (n <= 10) {
        long long fm = floor_k_phi_signed((long long)n - 11);
        long long res = (long long)fn - fm - 17;
        return (seq_t)res;
    } else {
        seq_t fm = floor_n_phi_unsigned(n - 11);
        seq_t res = fn - fm - (seq_t)17;
        return res;
    }
}
