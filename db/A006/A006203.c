/*
 * A006203  Discriminants of imaginary quadratic fields with class number 3
 *          (negated).
 * https://oeis.org/A006203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full, nice
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   sqfree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = n-th squarefree d == 3 (mod 4) for which the class number h(-d) is 3,
   where h(-d) is found by counting reduced binary quadratic forms of discriminant -d.
   Fields with odd class number have discriminant -4, -8 or -p, so this search covers
   every term. */
#include <stdbool.h>
#include "../utils/sqfree.h"

/* number of reduced forms (a,b,c), b^2 - 4ac = -d, |b| <= a <= c */
static long classno(long d)
{
  long h = 0, b, a;
  for (b = d & 1; 3 * b * b <= d; b += 2) {
    long m = (b * b + d) / 4;
    for (a = (b > 1 ? b : 1); a * a <= m; a++) {
      if (m % a) continue;
      {
        long c = m / a;
        if (c < a) continue;
        if (b == 0 || a == b || a == c) h += 1;
        else h += 2;
      }
    }
  }
  return h;
}

seq_t a(idx_t n)
{
  static long found[64];
  static int nfound = 0;
  static long next = 3;
  const long LIMIT = 20000;

  while ((idx_t)nfound < n && next <= LIMIT) {
    long d = next;
    next += 4;
    if (sqfree(d) && classno(d) == 3 && nfound < 64)
      found[nfound++] = d;
  }
  if (n < 1 || n > (idx_t)nfound) return (seq_t)0;
  return (seq_t)found[n - 1];
}
