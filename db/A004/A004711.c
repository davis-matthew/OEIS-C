/*
 * A004711  Positions of 1's in binary expansion of Pi/4.
 * https://oeis.org/A004711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = positions of 1's in binary expansion of Pi/4, extracted
 *            via Bailey–Borwein–Plouffe formula
 * Range:     u8 n <= 108, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

#define MAX_CACHED_HEX 100000

static int hex_cache[MAX_CACHED_HEX];
static int cache_len = 0;

static uint64_t modpow(uint64_t base, uint64_t exp, uint64_t modulus) {
    uint64_t result = 1;
    base %= modulus;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % modulus;
        }
        exp >>= 1;
        base = (base * base) % modulus;
    }
    return result;
}

static int hexdigit_pi(int d) {
    long double s = 0.0L;
    
    for (int k = 0; k <= d; k++) {
        uint64_t ak = 8LL * k + 1;
        uint64_t bk = 8LL * k + 4;
        uint64_t ck = 8LL * k + 5;
        uint64_t dk = 8LL * k + 6;
        
        uint64_t exp = d - k;
        long double r1 = (long double)modpow(16, exp, ak) / (long double)ak;
        long double r2 = (long double)modpow(16, exp, bk) / (long double)bk;
        long double r3 = (long double)modpow(16, exp, ck) / (long double)ck;
        long double r4 = (long double)modpow(16, exp, dk) / (long double)dk;
        
        s += 4.0L * r1 - 2.0L * r2 - r3 - r4;
        s = s - floorl(s);
    }
    
    for (int k = d + 1; k <= d + 50; k++) {
        long double ak = (long double)(8LL * k + 1);
        long double bk = (long double)(8LL * k + 4);
        long double ck = (long double)(8LL * k + 5);
        long double dk = (long double)(8LL * k + 6);
        
        long double term = powl(16.0L, (long double)(d - k)) *
                          (4.0L / ak - 2.0L / bk - 1.0L / ck - 1.0L / dk);
        s += term;
    }
    
    s = s - floorl(s);
    if (s < 0.0L) s += 1.0L;
    
    return (int)(16.0L * s);
}

static int get_hexdigit(int d) {
    if (d < cache_len) {
        return hex_cache[d];
    }
    if (d < MAX_CACHED_HEX) {
        while (cache_len <= d) {
            hex_cache[cache_len] = hexdigit_pi(cache_len);
            cache_len++;
        }
        return hex_cache[d];
    }
    return hexdigit_pi(d);
}

static int binarydigit_pi_fractional(int d) {
    int hex_idx = d / 4;
    int bit_pos = 3 - (d % 4);
    int hex_digit = get_hexdigit(hex_idx);
    return (hex_digit >> bit_pos) & 1;
}

static int binarydigit_pi4(int d) {
    if (d == 0 || d == 1) {
        return 1;
    }
    return binarydigit_pi_fractional(d - 2);
}

seq_t a(idx_t n) {
    seq_t count = 0;
    int d = 0;
    
    while (count < (seq_t)n && d < 10000000) {
        if (binarydigit_pi4(d)) {
            count++;
            if (count == (seq_t)n) {
                return (seq_t)(d + 1);
            }
        }
        d++;
    }
    
    return 0;
}
