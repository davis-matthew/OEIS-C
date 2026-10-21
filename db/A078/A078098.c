/*
 * A078098  Let u(1)=u(2)=1, u(3)=2n+1, u(k)=abs(u(k-1)-u(k-2)-u(k-3)); then
 *          for any n (u(k),u(k+1)) = (v(n),w(n)) for k large enough; sequence
 *          gives values of Max(v(n),w(n)).
 * https://oeis.org/A078098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A078098.
 * For a given n (1‑based), start the sequence u with
 *   u[1] = u[2] = 1,  u[3] = 2*n + 1
 *   u[k] = |u[k-1] - u[k-2] - u[k-3]|   for k ≥ 4
 *   The recurrence eventually reaches a 2‑cycle (a,b).
 *     a = u[k-2], b = u[k-1] when u[k] == u[k-2].
 * This function returns max(a,b), i.e. a(n). */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* index 0 is unspecified – return 0 */

    seq_t u1 = 1;                  /* u[k-2]  */
    seq_t u2 = 1;                  /* u[k-1]  */
    seq_t u3 = ((seq_t)n) * 2 + 1; /* u[k]    */

    /* iterate until the new value equals the value two steps back,
       which indicates the start of the 2‑cycle (a,b). */
    while (u3 != u1) {
        /* compute |u3 - u2 - u1| without underflow */
        seq_t sum = u1 + u2;
        seq_t nxt = (u3 >= sum) ? (u3 - sum) : (sum - u3);

        /* shift window */
        u1 = u2;
        u2 = u3;
        u3 = nxt;
    }

    /* u1 and u2 are the two numbers of the eventual 2‑cycle. */
    return (u1 > u2) ? u1 : u2;
}
