/*
 * A075261  y-value of the solution (x,y,z) to 3/(2n+1) = 1/x + 1/y + 1/z
 *          satisfying 0 < x < y < z, odd x, y, z and having the largest
 *          z-value. The x and z components are in A075260 and A075262.
 * https://oeis.org/A075261
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For n ≥ 2 let p = 2n+1.
   Iterate odd x > p/3.  For each such x, let
       a = 3x – p    (>0)
       b = p·x
   Every solution satisfies
           a·y = b + d ,   with   d | b
   where d = a·y – b is the divisor of b that has to be chosen.
   Hence we enumerate all divisors d of b, check whether (b + d) is
   divisible by a, obtain y = (b + d)/a, then compute
           z = b·y / d .
   If z is odd and > y we have a valid triple (x,y,z); we keep the
   one with the largest z.  The y‑coordinate of that triple is a(n). */

#include <stdbool.h>
#include <stddef.h>

/* sieve of primes up to 40000 – sufficient for √(max b) with n≤10000. */
static uint32_t primes[5000];
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void build_primes(void)
{
    if (primes_ready) return;
    const uint32_t limit = 40000;
    bool comp[40001] = {0};
    for (uint32_t i = 2; i * i <= limit; ++i)
        if (!comp[i])
            for (uint32_t j = i * i; j <= limit; j += i)
                comp[j] = true;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!comp[i]) primes[prime_cnt++] = i;
    primes_ready = true;
}

/* structure for prime factorisation */
typedef struct { uint32_t p; int exp; } Factor;

seq_t a(idx_t n)
{
    if ((idx_t)n < 2) return 0;           /* undefined for n<2 */

    build_primes();

    seq_t p = 2 * (seq_t)n + 1;           /* p = 2n+1, always > 0  */
    seq_t best_z = 0;
    seq_t best_y = 0;

    /* first odd x > p/3 */
    seq_t x_start = p / 3 + 1;
    if ((x_start & 1) == 0) x_start++;   /* make odd */

    for (seq_t x = x_start; ; x += 2)
    {
        /* stop if future x cannot improve best_z */
        if (best_z != 0) {
            seq_t lhs = p * x;
            seq_t rhs = best_z * (3 * x - p);
            if (lhs <= rhs) break;
        }

        seq_t a_coeff = 3 * x - p;        /* a > 0 */
        seq_t b = p * x;                  /* b = p·x */

        /* factorise b by trial division using pre‑computed primes */
        idx_t temp = (idx_t)b;            /* fits in idx_t (≤ ~4·10^8) */
        Factor f[64];
        size_t fc = 0;
        for (size_t i = 0; i < prime_cnt && (idx_t)primes[i] * primes[i] <= temp; ++i) {
            uint32_t pr = primes[i];
            if (temp % pr == 0) {
                int e = 0;
                while (temp % pr == 0) {
                    temp /= pr;
                    ++e;
                }
                f[fc].p = pr;
                f[fc].exp = e;
                ++fc;
            }
        }
        if (temp > 1) {                     /* remaining prime factor */
            f[fc].p = (uint32_t)temp;
            f[fc].exp = 1;
            ++fc;
        }

        /* build all divisors of b */
        seq_t divs[2000];
        size_t divcnt = 1;
        divs[0] = 1;
        for (size_t fi = 0; fi < fc; ++fi) {
            seq_t prime = f[fi].p;
            int exp = f[fi].exp;
            size_t oldcnt = divcnt;
            seq_t pow = 1;
            for (int e = 1; e <= exp; ++e) {
                pow *= prime;
                for (size_t j = 0; j < oldcnt; ++j) {
                    if (divcnt >= 2000) break; /* safety, should not happen */
                    divs[divcnt++] = divs[j] * pow;
                }
            }
        }

        /* test each divisor */
        for (size_t di = 0; di < divcnt; ++di) {
            seq_t d = divs[di];
            seq_t sum = d + b;
            if (sum % a_coeff != 0) continue;          /* y not integer */
            seq_t y = sum / a_coeff;
            if (y <= x) continue;                     /* need x < y */
            if ((y & 1) == 0) continue;                /* odd y only */

            seq_t z = b * y / d;                       /* integer by construction */
            if ((z & 1) == 0) continue;                /* odd z only */
            if (z <= y) continue;                     /* need y < z */

            if (z > best_z) {
                best_z = z;
                best_y = y;
            }
        }
    }

    return best_y;
}
