/*
 * A091797  Smallest k such that for 0 <= i < n, 1+i+n*(n-1)/2 divides k-i.
 * https://oeis.org/A091797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest k such that for 0 ≤ i < n, 1 + i + n*(n‑1)/2
 *            divides k – i
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean gcd for seq_t */
static seq_t seq_gcd(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Modular inverse of a modulo m.
   Guarantees that gcd(a,m)==1 and 0 <= a,m < 2^63. */
static seq_t mod_inv(seq_t a, seq_t m) {
    long long aa = (long long)a;
    long long mm = (long long)m;
    long long t = 0, newt = 1;
    long long r = mm, newr = aa;
    while (newr != 0) {
        long long q = r / newr;
        long long tmp = newt;
        newt = t - q * newt;
        t = tmp;
        tmp = newr;
        newr = r - q * newr;
        r = tmp;
    }
    if (r > 1) return 0;          /* inverse does not exist */
    if (t < 0) t += mm;
    return (seq_t)t;
}

/* A091797 sequence */
seq_t a(idx_t n) {
    if (n == 0) return 0;         /* undefined, safety value */

    /* C = n*(n-1)/2 */
    seq_t C = (seq_t)n * ((seq_t)n - 1) / 2;

    /* Initial congruence for i = 0: k ≡ 0 (mod C+1) */
    seq_t mod0 = C + 1;
    seq_t x = 0;                   /* current solution */
    seq_t M = mod0;                /* current modulus (lcm of processed moduli) */

    /* Combine the remaining congruences */
    for (idx_t i = 1; i < n; ++i) {
        seq_t mod = C + 1 + i;   /* modulus for this i */
        seq_t rem = i;           /* remainder for this i */

        seq_t g = seq_gcd(M, mod);

        /* Term needed for inverse: (M/g) mod (mod/g) */
        seq_t Mdivg = M / g;
        seq_t moddivg = mod / g;
        seq_t a_mod = Mdivg % moddivg;

        /* Difference (rem - x) mod mod, made positive */
        seq_t xm = x % mod;
        seq_t diff = (rem >= xm) ? (rem - xm) : (mod + rem - xm);

        /* (rem - x)/g */
        seq_t diff_div_g = diff / g;

        /* Inverse of (M/g) modulo (mod/g) */
        seq_t inv = mod_inv(a_mod, moddivg);

        /* t = ((rem - x)/g) * inv (mod mod/divg) */
        seq_t t = (diff_div_g * inv) % moddivg;

        /* New solution */
        x = x + M * t;
        M = Mdivg * mod;           /* new lcm */
        x %= M;
    }

    return x;
}
