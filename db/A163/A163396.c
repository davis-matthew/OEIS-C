/*
 * A163396  Primes in carryless arithmetic mod 10 in which all digits except
 *          the rightmost are even.
 * https://oeis.org/A163396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A163396: e‑type primes (carryless primes with all digits
   except the last even).  The function generates all such numbers in
   ascending order and tests carryless primality by verifying that the
   corresponding polynomial over Z/10Z is irreducible modulo 2 and
   modulo 5 (a prime in the product ring is prime in each component). */

#include <stdbool.h>

/* Check whether the polynomial represented by digits[0..deg] is irreducible
   over GF(p).  The polynomial is the carryless number: value = Σ digits[i]·x^i. */
static bool irreducible_mod_prime(const int *digits, int deg, int p)
{
    if (deg <= 1) return true;            /* degree 0 or 1 polynomials are irreducible */

    int max_check = deg / 2;               /* only divisors up to half the degree are needed */

    /* Temporary arrays for division; size 20 is more than enough for the maximum degree. */
    int f[20];
    int g[3];                              /* largest needed for degree 2 */

    for (int d = 1; d <= max_check; ++d) {
        if (d == 1) {                      /* monic polynomials x + a  (a = 0..p-1) */
            for (int a = 0; a < p; ++a) {
                g[0] = a;
                g[1] = 1;                  /* leading coefficient set to 1 */

                /* Copy and reduce coefficients modulo p. */
                for (int i = 0; i <= deg; ++i) f[i] = digits[i] % p;
                int rdeg = deg;
                while (rdeg >= 0 && f[rdeg] == 0) --rdeg;

                /* Long division: f = q*g + r;  test whether r==0. */
                while (rdeg >= d) {
                    int factor = f[rdeg];              /* leading coefficient of f (mod p) */
                    int shift  = rdeg - d;
                    for (int i = 0; i <= d; ++i) {
                        int idx = i + shift;
                        f[idx] -= factor * g[i];
                        f[idx] %= p;
                        if (f[idx] < 0) f[idx] += p;
                    }
                    while (rdeg >= 0 && f[rdeg] == 0) --rdeg;
                }
                if (rdeg < 0) return false;            /* divisible by g -> reducible */
            }
        } else if (d == 2) {                /* monic polynomials x² + a·x + b */
            for (int a = 0; a < p; ++a) {
                for (int b = 0; b < p; ++b) {
                    g[0] = a;
                    g[1] = b;
                    g[2] = 1;                  /* leading coefficient set to 1 */

                    for (int i = 0; i <= deg; ++i) f[i] = digits[i] % p;
                    int rdeg = deg;
                    while (rdeg >= 0 && f[rdeg] == 0) --rdeg;

                    while (rdeg >= d) {
                        int factor = f[rdeg];
                        int shift  = rdeg - d;
                        for (int i = 0; i <= d; ++i) {
                            int idx = i + shift;
                            f[idx] -= factor * g[i];
                            f[idx] %= p;
                            if (f[idx] < 0) f[idx] += p;
                        }
                        while (rdeg >= 0 && f[rdeg] == 0) --rdeg;
                    }
                    if (rdeg < 0) return false;        /* reducible */
                }
            }
        }
    }
    return true;           /* no divisor found -> irreducible */
}

/* Test whether x is a carryless (e‑type) prime. */
static bool is_emod_prime(seq_t x)
{
    int digits[20];
    int len = 0;
    while (x > 0) {
        int d = (int)(x % 10);
        digits[len++] = d;
        x /= 10;
    }
    if (len == 0) return false;            /* x == 0 */
    int deg = len - 1;

    if (!irreducible_mod_prime(digits, deg, 2)) return false;
    if (!irreducible_mod_prime(digits, deg, 5)) return false;
    return true;
}

/* Recursive generation of e‑type numbers.  Results are produced in ascending
   numeric order.  When the nth term is found the global flag stops further
   recursion. */
static idx_t target_n;
static idx_t count_seen;
static seq_t result_val;
static bool done_flag;

static void dfs_gen(int pos, int len, seq_t val)
{
    if (done_flag) return;
    if (pos == len) {
        if (is_emod_prime(val)) {
            ++count_seen;
            if (count_seen == target_n) {
                result_val = val;
                done_flag = true;
            }
        }
        return;
    }

    if (pos == 0) {                          /* leading (most significant) digit */
        static const int first[4] = {2,4,6,8};
        for (int i = 0; i < 4; ++i) {
            dfs_gen(pos+1, len, val * (seq_t)10 + (seq_t)first[i]);
            if (done_flag) return;
        }
    } else if (pos == len-1) {                /* least significant digit */
        static const int last[5] = {1,3,5,7,9};
        for (int i = 0; i < 5; ++i) {
            dfs_gen(pos+1, len, val * (seq_t)10 + (seq_t)last[i]);
            if (done_flag) return;
        }
    } else {                                  /* middle digits */
        static const int mid[5] = {0,2,4,6,8};
        for (int i = 0; i < 5; ++i) {
            dfs_gen(pos+1, len, val * (seq_t)10 + (seq_t)mid[i]);
            if (done_flag) return;
        }
    }
}

seq_t a(idx_t n)
{
    target_n = n;
    count_seen = 0;
    done_flag = false;

    for (int len = 2; ; ++len)   /* numbers start at length 2 */
    {
        dfs_gen(0, len, (seq_t)0);
        if (done_flag) break;
    }
    return result_val;
}
