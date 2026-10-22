/*
 * A006240  Row 4 of array in A212801.
 * https://oeis.org/A006240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (15^n + 3^n - 5^n - 1 + (2+i)^n + (2-i)^n - (6+3*i)^n -
 *            (6-3*i)^n)/4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

typedef struct {
    seq_t real;
    seq_t imag;
} complex_t;

static seq_t ipow(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result = result * base;
        }
        base = base * base;
        exp >>= 1;
    }
    return result;
}

static complex_t complex_mul(complex_t a, complex_t b) {
    complex_t result;
    result.real = a.real * b.real - a.imag * b.imag;
    result.imag = a.real * b.imag + a.imag * b.real;
    return result;
}

static complex_t complex_pow(complex_t base, idx_t exp) {
    complex_t result = {1, 0};
    while (exp > 0) {
        if (exp & 1) {
            result = complex_mul(result, base);
        }
        base = complex_mul(base, base);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t term1 = ipow(15, n);
    seq_t term2 = ipow(3, n);
    seq_t term3 = ipow(5, n);
    
    complex_t z1 = complex_pow((complex_t){2, 1}, n);
    complex_t z2 = complex_pow((complex_t){6, 3}, n);
    
    seq_t result = term1 + term2 - term3 - 1 + 2 * z1.real - 2 * z2.real;
    return result / 4;
}
