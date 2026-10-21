/*
 * A070294  Smallest k such that k==i (mod(i!+1)) i=1,2,3,...,n.
 * https://oeis.org/A070294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A070294: A(n) is the smallest integer k such that
   k ≡ i (mod (i!+1)) for all i = 1..n.
   The sequence has terms only up to n = 5; for larger n
   no such k exists, so the function returns 0. */
seq_t a(idx_t n)
{
    /* For n > 5 the congruence system has no solution */
    if (n > 5) return 0;

    seq_t k = 0;
    /* Brute‑force search; n ≤ 5 ⇒ k < 127050, so this loop
       finishes quickly even for 64‑bit seq_t. */
    for (;; ++k) {
        seq_t fact = 1;
        int ok = 1;
        for (idx_t i = 1; i <= n; ++i) {
            fact = fact * i;            /* i! */
            if ((k % (fact + 1)) != i) {
                ok = 0;
                break;
            }
        }
        if (ok) return k;
    }
}
