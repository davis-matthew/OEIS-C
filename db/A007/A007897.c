/*
 * A007897  a(n) is multiplicative with a(2) = 1; a(4) = 2; a(2^i) = 2^(i-2)+2
 *          if i>2; a(p^i) = 1+(p-1)*p^(i-1)/2 if prime p>2 and i>0.
 * https://oeis.org/A007897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative with a(2) = 1; a(4) = 2; a(2^i) =
 *            2^(i-2)+2 if i>2; a(p^i) = 1+(p-1)*p^(i-1)/2 if prime p>2 and
 *            i>0
 * Range:     u8 n <= 520, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000248_pow, pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw2.h"
#include "../utils/a000248_pow.h"


static seq_t compute_prime_power(idx_t p, idx_t e) {
    if (p == 2) {
        if (e == 1) return 1;
        if (e == 2) return 2;
        seq_t power = pw2(e - 2);
        return power + 2;
    } else {
        seq_t p_seq = (seq_t)p;
        seq_t half_pm1 = (p_seq - 1) / 2;
        seq_t pe_minus_1 = a000248_pow(p_seq, e - 1);
        seq_t result = 1 + half_pm1 * pe_minus_1;
        return result;
    }
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t temp = n;
    
    idx_t e2 = 0;
    while (temp % 2 == 0) {
        e2++;
        temp /= 2;
    }
    if (e2 > 0) {
        result *= compute_prime_power(2, e2);
    }
    
    for (idx_t p = 3; p * p <= temp; p += 2) {
        idx_t ep = 0;
        while (temp % p == 0) {
            ep++;
            temp /= p;
        }
        if (ep > 0) {
            result *= compute_prime_power(p, ep);
        }
    }
    
    if (temp > 1) {
        result *= compute_prime_power(temp, 1);
    }
    
    return result;
}
