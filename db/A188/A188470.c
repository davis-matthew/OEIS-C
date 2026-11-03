/*
 * A188470  a(n) = [5r]-[nr]-[5r-nr], where r=(1+sqrt(5))/2 and []=floor.
 * https://oeis.org/A188470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [5r] - [nr] - [5r-nr], r = (1+sqrt(5))/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 165 terms through n = 7983 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute floor(n * φ) for non‑negative n, where φ = (1+√5)/2.
   Uses integer arithmetic only. */
static seq_t floor_n_phi(idx_t n) {
    if (n == 0) return 0;
    unsigned __int128 nn = (unsigned __int128)n * n;    /* n²   */
    unsigned __int128 prod = (unsigned __int128)5 * nn; /* 5·n² */
    /* integer square root: floor(sqrt(prod)) */
    unsigned __int128 low = 0, high = (unsigned __int128)1 << 64;
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid * mid <= prod) low = mid;
        else high = mid - 1;
    }
    unsigned __int128 t = low;                 /* floor(n√5)  */
    unsigned __int128 a = (unsigned __int128)n + t;  /* n + floor(n√5)  */
    return (seq_t)(a >> 1);                    /* (n + floor(n√5))/2 = floor(nφ) */
}

/* floor(k * φ) for signed integers k.  For k < 0 the result is negative. */
static int64_t floor_phi_signed(int64_t k) {
    if (k >= 0) {
        return (int64_t)floor_n_phi((idx_t)k);
    } else {
        idx_t m = (idx_t)(-k);
        seq_t pos = floor_n_phi(m); /* floor(m φ)   */
        return -((int64_t)pos + 1); /* floor(-m φ) = -(floor(mφ)+1) */
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* behavior below offset is irrelevant */

    const seq_t FLOOR_5R = 8;          /* floor(5r) = 8 */
    int64_t fnr    = (int64_t)floor_n_phi(n);   /* floor(n r)   */
    int64_t f5nmnr = floor_phi_signed((int64_t)5 - (int64_t)n); /* floor((5-n)r) */

    int64_t result = (int64_t)FLOOR_5R - fnr - f5nmnr;
    return (seq_t)result;                 /* 0 or 1 */
}
